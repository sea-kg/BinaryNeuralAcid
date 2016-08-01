#include <iostream>
#include "vertex_graph.h"
#include "helpers.h"
#include "memory.h"
#include "memoryitem.h"
#include "tests/tests.h"
#include <QString>
#include <QList>
#include <QFile>
#include <QTime>
#include <QVector>

#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QCoreApplication>

void print_help(QVector<QString> &vParams) {
	std::cout << "\n"
		<< "  Please usage: " << vParams[0].toStdString() << " [command] [parameters]\n"
		<< "\t --reverse <hash>           - try reverse hash\n"
		<< "\t --run-tests                - run tests\n"
		<< "\t --resetpersents            - reset last persents from every bit and create missing files\n" 
        << "\t --memory <filename>        - generate file with memory\n"
        << "\t --training <memoryname>    - generate file with memory\n"
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

    if(vParams.size() > 2 && vParams[1] == "--memory"){
        reverse_hash::Memory *pMemory = new reverse_hash::Memory();
        pMemory->generateData(10000);
        pMemory->save(vParams[2]);
        std::cout << "Completed (10000 items)\n\n";
        return 0;
    }

	if(vParams.contains("--run-tests")){
		runtests();
		return 0;
	}

	if(vParams.size() > 2 && vParams[1] == "--reverse"){
		QString hash = vParams[2];
		std::cout << "Try revert hash: " << hash.toStdString() << "\n";
		QVector<bool> vOutput;
		QVector<bool> vInput;
		reverse_hash::convertHEXStringToVBool(hash, vInput, 128);
		// int nCount = 55*8;
		int nCount = 8;
		for (int i = 0; i < nCount; i++) {
            std::cout << "bit (" << i << ") ... ";
            bool bResult = false;
			QString filename = "md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			QFile file(filename);
			if(file.exists()){
				reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
				pVertexGraph->loadFromFile(filename);
				pVertexGraph->setIn(vInput);
				bResult = pVertexGraph->out();
            }else{
				std::cerr << "Error: File '" << filename.toStdString() << "'does not exists\n";
			}
            vOutput.push_back(bResult);
            std::cout << "OK " << (bResult ? "1" : "0") << "\n";
		}
		std::cout << "Result: " << reverse_hash::convertVBoolHEXString(vOutput).toStdString() << "\n\n";
		return 0;
	}

	// reset persents
	if(vParams.contains("--resetpersents")){
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString filename = "md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
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

	if(vParams.contains("--training")){
		reverse_hash::Memory *pMemory = new reverse_hash::Memory();
		pMemory->load("md5/memory_md5_10000.rhmem");

		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString filename = "md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
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
				}
				
				nPersent = (nSuccessCount * 100) / (nMemorySize);
				if(nPersent > pVertexGraph->lastSuccessPersents()){
					std::cout << " * New persent result: " << nPersent << "% [" << nSuccessCount << " / " << nMemorySize << "] \r";
					pVertexGraph->setLastSuccessPersents(nPersent);
					pVertexGraph->saveToFile(filename);
				}else{
					pVertexGraph->loadFromFile(filename);
					nPersent = pVertexGraph->lastSuccessPersents();
					std::cout << " * Last persent result: " << pVertexGraph->lastSuccessPersents() << "% [" << nSuccessCount << " / " << nMemorySize << "] \r";
					pVertexGraph->randomChanges(50);
				}
			}
			std::cout << " * Result: " << pVertexGraph->lastSuccessPersents() << "%                              \n";
		};
		return 0;
	}

	print_help(vParams);
    return 0;
}
