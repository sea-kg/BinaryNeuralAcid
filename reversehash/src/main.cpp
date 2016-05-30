#include <iostream>
#include "graph.h"
#include "vertex_graph.h"
#include "helpers.h"
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

	if(vParams.size() > 2 && vParams[1] == "reverse"){
		QString hash = vParams[2];
		QVector<bool> vOutput;
		QVector<bool> vInput;
		crack_hash::convertHEXStringToVBool(hash, vInput, 128);
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
            std::cout << "bit (" << i << ") ... ";
            bool bResult = false;
			QString base_filename = "md5/bit" + QString::number(i).rightJustified(3, '0');
			QString filename = base_filename + ".graph";
			QFile file(filename);
			if(file.exists()){
				crack_hash::Graph *pGraph = new crack_hash::Graph();
				pGraph->load(filename);
                crack_hash::Calculate *pCalc = new crack_hash::Calculate(pGraph);
				QMap<QString, bool> input;
				for (int i = 0; i < 128; i++) {
					QString in = "i" + QString::number(i, 16).rightJustified(2, '0');
					input[in] = vInput[i];
				}
                bResult = pCalc->calc("o00", input);
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
		// pVertexGraph->randomChanges(50);
        pVertexGraph->out();
        
        pVertexGraph->save("test.vertexgraph");
		pVertexGraph->saveDot("test.dot");
        pVertexGraph->load("test.vertexgraph");
        
        pVertexGraph->save("test1.vertexgraph");
        pVertexGraph->saveDot("test1.dot");
        pVertexGraph->load("test1.vertexgraph");
        
        pVertexGraph->save("test2.vertexgraph");
        pVertexGraph->saveDot("test2.dot");
        QString result = pVertexGraph->conv2dot();
        // std::cout << result.toStdString() << "\n";
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
				std::cout << "new graph: " << filename.toStdString() << "\n";
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
