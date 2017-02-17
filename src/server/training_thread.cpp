#include "training_thread.h"

#include "../vertex_graph.h"
#include "../helpers.h"
#include "../memory.h"
#include "../memoryitem.h"
#include <QFile>

TrainingThread::TrainingThread(IWebSocketServer *pWebSocketServer){
	m_pWebSocketServer = pWebSocketServer;
	m_nSleep = 5;
}

void TrainingThread::run(){
	qDebug().noquote().nospace() << "Start Training Thread";
	reverse_hash::Memory *pMemory = new reverse_hash::Memory();
	pMemory->load("/usr/share/reversehashd/md5/memory_md5_10000.rhmem");
	
	while(true){
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString bitid = "bit" + QString::number(i).rightJustified(3, '0');
			QString filename = "/usr/share/reversehashd/md5/" + bitid + ".vertexgraph";
			QFile file(filename);
			if (!file.exists()) {
				this->sendMessage(bitid, filename + " file did not found");
				QThread::sleep(m_nSleep);
				continue;
			}
			reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
			pVertexGraph->loadFromFile(filename);
			this->sendMessage(bitid, "Loaded file: " + filename);
			
			this->sendMessage(bitid, "Processing... ");
			
			
			int nMemorySize = pMemory->size();
			int nPersent = pVertexGraph->lastSuccessPersents();
			int nExperiments = 0;
			while(nPersent < 90 && nExperiments < 100){
				QThread::sleep(m_nSleep);
				nExperiments++;
				int nSuccessCount = 0;

				for (int t = 0; t < nMemorySize; t++){
					reverse_hash::MemoryItem memoryItem = pMemory->at(t);
					pVertexGraph->setIn(memoryItem.outputToVectorBool());
					bool b = pVertexGraph->out();
					if(b == memoryItem.inputToVectorBool()[i]){
						nSuccessCount++;
					}
					
					if(t > 0 && t % 1000 == 0){
						nPersent = (nSuccessCount * 100) / (t);
						this->sendMessage(bitid, "Processed experiment: #" + QString::number(nExperiments) + " (" + QString::number(pVertexGraph->lastSuccessPersents()) + "%)");
						QThread::sleep(1);
					}
				}
				
				nPersent = (nSuccessCount * 100) / (nMemorySize);
				if(nPersent > pVertexGraph->lastSuccessPersents()){
					this->sendMessage(bitid, "New persent result: " + QString::number(nPersent) + "% [" + QString::number(nSuccessCount) + "/" + QString::number(nMemorySize) + "]");
					pVertexGraph->setLastSuccessPersents(nPersent);
					pVertexGraph->saveToFile(filename);
				}else{
					pVertexGraph->loadFromFile(filename);
					nPersent = pVertexGraph->lastSuccessPersents();
					this->sendMessage(bitid, "Last persent result: " + QString::number(pVertexGraph->lastSuccessPersents()) + "% {" + QString::number(nSuccessCount) + "/" + QString::number(nMemorySize) + "}");
					pVertexGraph->randomChanges(13);
				}
			}
			this->sendMessage(bitid, "Result: " + QString::number(pVertexGraph->lastSuccessPersents()) + "%");
			QThread::sleep(m_nSleep);
		}
		QThread::sleep(m_nSleep);
	}
	qDebug().noquote().nospace() << "Stop Training Thread";
}

void TrainingThread::sendMessage(QString bitid, QString message){
	qDebug().noquote().nospace() << "Training Thread: [" << bitid << "] " << message;
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue("training");
	jsonData["rid"] = QJsonValue(0);
	jsonData["bitid"] = bitid;
	jsonData["result"] = "OK";
	jsonData["status"] = message;
	// Redesign use emit
	// m_pWebSocketServer->sendToAll(jsonData);
}
