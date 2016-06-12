#include <iostream>
#include "graph.h"
#include "vertex_graph.h"
#include "helpers.h"
#include "memory.h"
#include "memoryitem.h"
#include <QString>
#include <QList>
#include <QFile>
#include <QTime>
#include <QVector>

#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QCoreApplication>

void print_help(QVector<QString> &vParams) {
	std::cout << "Please usage: " << vParams[0].toStdString() << " [command] [parameters]\n"
		<< "\t --reverse <hash>\n"
		<< "\t --testvertexgraph\n" 
		<< "\t --run <memoryfile>\n" 
        << "\t --memory <filename> - generate file with memory\n";
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
        crack_hash::Memory *pMemory = new crack_hash::Memory();
        pMemory->generateData(10000);
        pMemory->save(vParams[2]);
        std::cout << "Completed (10000 items)\n\n";
        return 0;
    }

	if(vParams.contains("--runtests")){
		QMap<QString, QString> tests;
		tests["202CB962AC59075B964B07152D234B70"] = "123";
		tests["92BA7B54A295FED5060A2BA44A72E595"] = "66^lvp/-!A";
		tests["b885ca6d1c4e8231928ef2af5737426c"] = "YN!KAHfPjFU[\">IwHL";
		
		/*crack_hash::Memory *pMemory = new crack_hash::Memory();
		pMemory->load("md5/memory_md5_10000.rhmem");
		crack_hash::MemoryItem memoryItem = pMemory->at(1);
		tests[memoryItem.output.toHex()] = memoryItem.input.toHex();*/
		
		foreach(QString key, tests.keys()){
			QVector<bool> vInput;
			crack_hash::convertHEXStringToVBool(key, vInput, 128);
			
			QVector<bool> vOutputExpected;
			QByteArray originalString = tests.value(key).toUtf8();
			crack_hash::convertArrayToVBool(originalString, vOutputExpected, originalString.size()*8);
		 
			int nCount = 55*8;
			QVector<bool> vOutput;
			for (int i = 0; i < nCount; i++) {
				bool bResult = false;
				QString filename = "md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
				QFile file(filename);
				if(file.exists()){
					reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
					pVertexGraph->load(filename);
					pVertexGraph->setIn(vInput);
					vOutput.push_back(pVertexGraph->out());
				}else{
					std::cerr << "Error: File '" << filename.toStdString() << "'does not exists\n";
				}
			}

			int min = std::min(vOutputExpected.size(), vOutput.size());
			int nCheck = 0;
			for(int i = 0; i < min; i++){
				if(vOutput[i] == vOutputExpected[i])
					nCheck++;
			}
			for(int i = min; i < vOutput.size(); i++ ){
				if(vOutput[i] == false)
					nCheck++;
			}
			int  nPersent = (nCheck * 100) / nCount;
			std::cout << key.toStdString() << " => " << tests.value(key).toStdString() << "; reverted " << nPersent << "% [" << nCheck << "/" << nCount << " bits]\n";
		}
		return 0;
	}


	if(vParams.size() > 2 && vParams[1] == "--reverse"){
		QString hash = vParams[2];
		std::cout << "Try revert hash: " << hash.toStdString() << "\n";
		QVector<bool> vOutput;
		QVector<bool> vInput;
		crack_hash::convertHEXStringToVBool(hash, vInput, 128);
		// int nCount = 55*8;
		int nCount = 8;
		for (int i = 0; i < nCount; i++) {
            std::cout << "bit (" << i << ") ... ";
            bool bResult = false;
			QString filename = "md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			QFile file(filename);
			if(file.exists()){
				reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
				pVertexGraph->load(filename);
				pVertexGraph->setIn(vInput);
				bResult = pVertexGraph->out();
            }else{
				std::cerr << "Error: File '" << filename.toStdString() << "'does not exists\n";
			}
            vOutput.push_back(bResult);
            std::cout << "OK " << (bResult ? "1" : "0") << "\n";
		}
		std::cout << "Result: " << crack_hash::convertVBoolHEXString(vOutput).toStdString() << "\n\n";
		return 0;
	}

	if(vParams.contains("--testvertexgraph")){
		reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
		pVertexGraph->genBase();
		pVertexGraph->out();
		pVertexGraph->randomChanges(50);
        pVertexGraph->out();
        
        pVertexGraph->save("test/test.vertexgraph");
		pVertexGraph->saveDot("test/test.dot");
        pVertexGraph->load("test/test.vertexgraph");
        
        pVertexGraph->save("test/test1.vertexgraph");
        pVertexGraph->saveDot("test/test1.dot");
        pVertexGraph->load("test/test1.vertexgraph");
        
        pVertexGraph->save("test/test2.vertexgraph");
        pVertexGraph->saveDot("test/test2.dot");
        QString result = pVertexGraph->conv2dot();
        // std::cout << result.toStdString() << "\n";
		return 0;
	}

	if(vParams.contains("--resetpersents")){
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString filename = "md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
			QFile file(filename);
			if (!file.exists()) {
				pVertexGraph->genBase();
				pVertexGraph->save(filename);
				std::cout << "Created new file: " << filename.toStdString() << "\n";
			}else{
				pVertexGraph->load(filename);
				std::cout << "Loaded file: " << filename.toStdString() << "\n";
			}
			pVertexGraph->setLastSuccessPersents(0);
			pVertexGraph->save(filename);
		}
		return 0;
	}

	if(vParams.contains("--training")){
		crack_hash::Memory *pMemory = new crack_hash::Memory();
		pMemory->load("md5/memory_md5_10000.rhmem");
		
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString filename = "md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
			QFile file(filename);
			if (!file.exists()) {
				pVertexGraph->genBase();
				pVertexGraph->save(filename);
				std::cout << "Created new file: " << filename.toStdString() << "\n";
			}else{
				pVertexGraph->load(filename);
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
					crack_hash::MemoryItem memoryItem = pMemory->at(t);
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
					pVertexGraph->save(filename);
				}else{
					pVertexGraph->load(filename);
					nPersent = pVertexGraph->lastSuccessPersents();
					std::cout << " * Last persent result: " << pVertexGraph->lastSuccessPersents() << "% [" << nSuccessCount << " / " << nMemorySize << "] \r";
					pVertexGraph->randomChanges(50);
				}
			}
			std::cout << " * Result: " << pVertexGraph->lastSuccessPersents() << "%                              \n";
		};
		return 0;
	}

	// count of bits in source string
	if(vParams.size() > 2 && vParams[1] == "--run"){
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString base_filename = "md5/bit" + QString::number(i).rightJustified(3, '0');
			QString filename = base_filename + ".graph";
			QString filename_dot = base_filename + ".dot";
			QString filename_persent = base_filename + ".persent";
			QFile file(filename);
			crack_hash::Graph *pGraph = new crack_hash::Graph();
			crack_hash::Memory *pMemory = new crack_hash::Memory();
			pMemory->load("memory_md5_10000.rhmem");

			int nPersent = 0;
			if (!file.exists()) {
				pGraph->generateRandomGraph();
				pGraph->save(filename);
				pGraph->exportToFileDot(filename_dot);
				std::cout << "New graph: " << filename.toStdString() << "\n";
				nPersent = crack_hash::testGraph(pGraph, pMemory, i);
				crack_hash::savePersent(filename_persent, nPersent);
				std::cout << i << " * Result: " << nPersent << "%\n";
			} else {
				pGraph->load(filename);
				std::cout << "load graph: " << filename.toStdString() << "\n";
				nPersent = crack_hash::loadPersent(filename_persent);
				if (nPersent == 0) {
					nPersent = crack_hash::testGraph(pGraph, pMemory, i);
					crack_hash::savePersent(filename_persent, nPersent);
				}
				std::cout << i << " * Result: " << nPersent << "%\n";
			}

			while (nPersent < 90) {
				pGraph->modifyRandomGraph();
				int nNewPersent = crack_hash::testGraph(pGraph, pMemory, 0);
				if (nNewPersent > nPersent) {
					pGraph->save(filename);
					pGraph->exportToFileDot(filename_dot);
					crack_hash::savePersent(filename_persent, nNewPersent);
					nPersent = nNewPersent;
				} else {
					pGraph->load(filename);
				}
				std::cout << i << " * New Result: " << nNewPersent << "%\n";
			}
		}
		return 0;
	}
	print_help(vParams);
    return 0;
}
