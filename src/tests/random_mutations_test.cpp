#include "random_mutations_test.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>

QString RandomMutations_Test::name(){
	return "RemoveRandomVertex_Test";
};

void RandomMutations_Test::printStat(int i, int cicles, int max){
	std::cout << "\r\t Calculates " << i << " (cicles: " << cicles << " / " << max << ") ";
}

bool RandomMutations_Test::run(){
    /*VertexGraph vg(128);
	vg.genBase();
	int nMax = vg.countOfVertextes() - 128 - 1;
	int cicles = 0;
	std::cout << "\n";
	printStat(0, cicles, nMax);
	
	for(int i = 0; i < nMax; i++){
		this->printStat(i, cicles, nMax);
		VertexGraph *pVertexGraphClone = vg.clone();
		int nBeforeCount = pVertexGraphClone->countOfVertextes();
		pVertexGraphClone->randomRemoveVertex();
		int nAfterCount = pVertexGraphClone->countOfVertextes();
		if(!pVertexGraphClone->findCicles()){
			pVertexGraphClone->out();
			if(nAfterCount != nBeforeCount-1){
				std::cout << "\r\tDifferent count of vertexes (expexted " << (nBeforeCount-1) << ", but got: " << nAfterCount << ")\n";
			}
			vg.copy(pVertexGraphClone);
		}else{
			cicles++;
		}
		delete pVertexGraphClone;
	}
	this->printStat(nMax, cicles, nMax);
    std::cout << "\n";*/
	return true;
};

