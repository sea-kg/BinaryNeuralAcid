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
	// TODO
	/*reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
	pVertexGraph->genBase();
	int nMax = 1000;
	int cicles = 0;
	std::cout << "\n";
	printStat(0, cicles, nMax);
	
	for(int i = 0; i < nMax; i++){
		printStat(i, cicles, nMax);
		pVertexGraph->changeRandomOperation();
		reversehash::VertexGraph *pVertexGraphClone = pVertexGraph->clone();
		pVertexGraphClone->swapRandomVertextIns();
		if(!pVertexGraphClone->findCicles()){
			pVertexGraphClone->out();
			pVertexGraph->copy(pVertexGraphClone);
		}else{
			cicles++;
		}
		delete pVertexGraphClone;
	}
	printStat(nMax, cicles, nMax);
	std::cout << "\n";*/
	return true;
};

