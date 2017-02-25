#include "random_add_vertex_test.h"
#include "../vertex_graph.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>

QString RandomAddVertex_Test::name(){
	return "RandomAddVertex_Test";
};

void RandomAddVertex_Test::printStat(int i, int cicles, int max){
	std::cout << "\r\t Calculates " << i << " (cicles: " << cicles << " / " << max << ") ";
}

bool RandomAddVertex_Test::run(){
	VertexGraph vg(128);
	vg.genBase();
	int nMax = 250;
	int cicles = 0;
	std::cout << "\n";
	printStat(0, cicles, nMax);
	
	for(int i = 0; i < nMax; i++){
		printStat(i, cicles, nMax);
		vg.changeRandomOperation();
		VertexGraph *pVertexGraphClone = vg.clone();
		pVertexGraphClone->randomAddVertex();
		if(!pVertexGraphClone->findCicles()){
			pVertexGraphClone->out();
			vg.copy(pVertexGraphClone);
		}else{
			cicles++;
		}
		delete pVertexGraphClone;
	}
	printStat(nMax, cicles, nMax);
	std::cout << "\n";
	return true;
};

