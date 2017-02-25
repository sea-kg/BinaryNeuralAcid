#include <iostream>
#include "vertex_graph.h"
#include "helpers.h"
#include "memory.h"
#include "memoryitem.h"
#include "tests/tests.h"
#include "server/websocketserver.h"
#include <QString>
#include <QList>
#include <QFile>
#include <QTime>
#include <QVector>
#include <QTextStream>


#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QCoreApplication>

void print_help(QVector<QString> &vParams) {
	std::cout << "\n"
		<< "  Please usage: " << vParams[0].toStdString() << " [command] [parameters]\n"
		<< "\t --run-tests                - run tests\n"
		<< "\t --resetpersents            - reset last persents from every bit and create missing files\n" 
        << "\t --training <memoryname>    - generate file with memory\n"
        << "\t --server                   - start server\n"
        << "\n";
};

int main(int argc, char* argv[])
{
	// Seed the random generator with current time
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());

	QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("reversehash");

	QVector<QString> vParams;
	for(int i = 0; i < argc; i++){
		vParams.push_back(QString(argv[i]));
	}

    if(vParams.contains("--help")){
        print_help(vParams);
        return 0;
    }

	if(vParams.contains("--run-tests")){
		runtests();
		return 0;
	}

	// reset persents
	if(vParams.contains("--resetpersents")){
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
			QFile file(filename);
			if (!file.exists()) {
				pVertexGraph->genBase();
				pVertexGraph->saveToFile(filename);
				std::cout << "Created new file: " << filename.toStdString() << "\n";
			}else{
				pVertexGraph->loadFromFile(filename);
				std::cout << "Loaded file: " << filename.toStdString() << "\n";
			}
			pVertexGraph->setLastSuccessPersents(0);
			pVertexGraph->saveToFile(filename);
		}
		return 0;
	}
	
	if(vParams.contains("--server")){
		qDebug() << "Server starting on 888 port";
		WebSocketServer *server = new WebSocketServer(43735, true);
		QObject::connect(server, &WebSocketServer::closed, &app, &QCoreApplication::quit);

		// SyncronizatonData *pSyncronizatonData = new SyncronizatonData();
		// pSyncronizatonData->Upload();
		// "md5/memory_md5_10000.rhmem"
		
		return app.exec();
	}

	if(vParams.contains("--training")){
		reverse_hash::Memory *pMemory = new reverse_hash::Memory();
		pMemory->load("/usr/share/reversehashd/md5/memory_md5_10000.rhmem");

		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
			QFile file(filename);
			if (!file.exists()) {
				pVertexGraph->genBase();
				pVertexGraph->saveToFile(filename);
				std::cout << "Created new file: " << filename.toStdString() << "\n";
			}else{
				pVertexGraph->loadFromFile(filename);
				std::cout << "Loaded file: " << filename.toStdString() << "\n";
			}

			std::cout << " * Processing... \r";
			
			int nMemorySize = pMemory->size();
			int nPersent = 0; //pVertexGraph->lastSuccessPersents();
			int nExperiments = 0;
			while(nPersent < 90 && nExperiments < 100){
				nExperiments++;
				int nSuccessCount = 0;

				for (int t = 0; t < nMemorySize; t++){
					reverse_hash::MemoryItem memoryItem = pMemory->at(t);
					pVertexGraph->setIn(memoryItem.outputToVectorBool());
					bool b = pVertexGraph->out();
					if(b == memoryItem.inputToVectorBool()[i]){
						nSuccessCount++;
					}
					
					if(t > 0 && t % 100 == 0){
						nPersent = (nSuccessCount * 100) / (t);
						std::cout << " * Processed (Experiment: #" << nExperiments << " [" << pVertexGraph->lastSuccessPersents() << "%])                              \r";
					}
				}
				
				nPersent = (nSuccessCount * 100) / (nMemorySize);
				if(nPersent > pVertexGraph->lastSuccessPersents()){
					std::cout << " * New persent result: " << nPersent << "% [" << nSuccessCount << " / " << nMemorySize << "]                               \r";
					pVertexGraph->setLastSuccessPersents(nPersent);
					pVertexGraph->saveToFile(filename);
				}else{
					pVertexGraph->loadFromFile(filename);
					nPersent = pVertexGraph->lastSuccessPersents();
					std::cout << " * Last persent result: " << pVertexGraph->lastSuccessPersents() << "% [" << nSuccessCount << " / " << nMemorySize << "]                          \r";
					pVertexGraph->randomChanges(13);
				}
			}
			std::cout << " * Result: " << pVertexGraph->lastSuccessPersents() << "%                              \n";
		};
		return 0;
	}

	print_help(vParams);
    return 0;
}
