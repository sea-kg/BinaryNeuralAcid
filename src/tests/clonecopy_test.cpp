#include "clonecopy_test.h"
#include "../vertex_graph.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>

QString CloneCopy_Test::name(){
	return "CloneCopy_Test";
};

bool CloneCopy_Test::run(){
	reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
	pVertexGraph->genBase();
	int nMax = 10000;
	std::cout << "\t Calculates 0";
	for(int i = 0; i < nMax; i++){
		std::cout << "\r\t Calculates " << QString::number(i).toStdString();
		reversehash::VertexGraph *pVertexGraphClone = pVertexGraph->clone();
		// pVertexGraphClone->changeRandomOperation();
		pVertexGraph->copy(pVertexGraphClone);
		delete pVertexGraphClone;
	}
	delete pVertexGraph;
	std::cout << "\r\t Calculates " << nMax << "\n";
	return true;
};

