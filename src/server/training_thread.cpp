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
		QVector<TrainingThreadItem *> list = this->sortedList();
		for(int i = 0; i < list.size(); i++){
			TrainingThreadItem *pItem = list[i];
			QString bitid = pItem->bitid();
			QString filename = pItem->filename();
			QFile file(filename);
			if (!file.exists()) {
				TrainingThreadMessage msg(pItem);
				msg.setMessage(filename + " file did not found");
				this->sendMessage(msg);
				QThread::sleep(m_nSleep);
				continue;
			}
			VertexGraph vg(128);
			vg.loadFromFile(filename);
			
			{
				TrainingThreadMessage msg(pItem);
				msg.setMessage("Training... ");
				this->sendMessage(msg);
			}

			int nMemorySize = pMemory->size();
			int nPersent = vg.lastSuccessPersents();
			int nExperiments = 0;
			while(nPersent < 90 && nExperiments < 100){
				QThread::sleep(m_nSleep);
				nExperiments++;
				int nSuccessCount = 0;

				for (int t = 0; t < nMemorySize; t++){
					reverse_hash::MemoryItem memoryItem = pMemory->at(t);
					vg.setIn(memoryItem.outputToVectorBool());
					bool b = vg.out();
					if(b == memoryItem.inputToVectorBool()[pItem->id()]){
						nSuccessCount++;
					}
					
					if(t > 0 && t % 1000 == 0){
						nPersent = (nSuccessCount * 100) / (t);

						TrainingThreadMessage msg(pItem);
						msg.setMessage("Training...");
						msg.setMaxExperiments(100);
						msg.setCompletedExperiments(nExperiments);
						msg.setPercent(nPersent);
						this->sendMessage(msg);
						
						QThread::sleep(1);
					}
				}

				nPersent = (nSuccessCount * 100) / (nMemorySize);
				if(nPersent > vg.lastSuccessPersents()){
					TrainingThreadMessage msg(pItem);
					msg.setMessage("New percent result set!");
					msg.setLastSuccessPersents(nPersent);
					msg.setMaxExperiments(100);
					msg.setCompletedExperiments(nExperiments);
					this->sendMessage(msg);
					
					vg.setLastSuccessPersents(nPersent);
					vg.saveToFile(filename);
				}else{
					TrainingThreadMessage msg(pItem);
					msg.setMessage("Reloading graph.");
					msg.setLastSuccessPersents(vg.lastSuccessPersents());
					this->sendMessage(msg);
					
					vg.loadFromFile(filename);
					nPersent = vg.lastSuccessPersents();
					vg.randomChanges(13);
				}
			}
			
			{
				TrainingThreadMessage msg(pItem);
				msg.setMessage("Result: " + QString::number(vg.lastSuccessPersents()) + "%");
				msg.setMaxExperiments(100);
				msg.setCompletedExperiments(100);
				this->sendMessage(msg);
			}
			QThread::sleep(m_nSleep);
		}
		QThread::sleep(m_nSleep);
	}
	qDebug().noquote().nospace() << "Stop Training Thread";
}

void TrainingThread::sendMessage(TrainingThreadMessage &msg){
	if(m_lastMessage.equals(msg)){
		return;
	}
	
	m_lastMessage = msg;
	qDebug().noquote().nospace() << "Training Thread: [" << msg.bitid() << "] " << msg.message();

	m_pWebSocketServer->sendToAll(msg.toJson());
}

bool sortFunctionTTI( const TrainingThreadItem * e1, const TrainingThreadItem * e2 ) {
	if(e1->percent() < e2->percent()){
		return true;
	}else if(e1->percent() > e2->percent()){
		return false;
	}
    return e1->id() < e2->id();
}

QVector<TrainingThreadItem *> TrainingThread::sortedList(){
	QVector<TrainingThreadItem *> result;
	int nCount = 55*8;
	for (int i = 0; i < nCount; i++) {
		result.push_back(new TrainingThreadItem(i));
	}
	qSort ( result.begin(), result.end(), sortFunctionTTI );
	return result;
}
