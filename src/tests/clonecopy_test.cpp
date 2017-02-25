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
	VertexGraph vg(128);
	vg.genBase();
	int nMax = 10000;
	std::cout << "\t Calculates 0";
	for(int i = 0; i < nMax; i++){
		std::cout << "\r\t Calculates " << QString::number(i).toStdString();
		VertexGraph *pVertexGraphClone = vg.clone();
		pVertexGraphClone->changeRandomOperation();
		vg.copy(pVertexGraphClone);
		delete pVertexGraphClone;
	}
	std::cout << "\r\t Calculates " << nMax << "\n";
	return true;
};

