#include "random_remove_vertex_test.h"
#include "../vertex_graph.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>

QString RandomRemoveVertex_Test::name(){
	return "RemoveRandomVertex_Test";
};

void RandomRemoveVertex_Test::printStat(int i, int cicles, int max){
	std::cout << "\r\t Calculates " << i << " (cicles: " << cicles << " / " << max << ") ";
}

bool RandomRemoveVertex_Test::run(){
	reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
	pVertexGraph->genBase();
	int nMax = pVertexGraph->countOfVertextes() - 128 - 1;
	int cicles = 0;
	std::cout << "\n";
	printStat(0, cicles, nMax);
	
	for(int i = 0; i < nMax; i++){
		this->printStat(i, cicles, nMax);
		reversehash::VertexGraph *pVertexGraphClone = pVertexGraph->clone();
		pVertexGraph->countOfVertextes();
		int nBeforeCount = pVertexGraphClone->countOfVertextes();
		pVertexGraphClone->randomRemoveVertex();
		int nAfterCount = pVertexGraphClone->countOfVertextes();
		if(!pVertexGraphClone->findCicles()){
			pVertexGraphClone->out();
			if(nAfterCount != nBeforeCount-1){
				std::cout << "\r\tDifferent count of vertexes (expexted " << (nBeforeCount-1) << ", but got: " << nAfterCount << ")\n";
			}
			pVertexGraph->copy(pVertexGraphClone);
		}else{
			cicles++;
		}
		delete pVertexGraphClone;
	}
	this->printStat(nMax, cicles, nMax);
	std::cout << "\n";
	return true;
};

