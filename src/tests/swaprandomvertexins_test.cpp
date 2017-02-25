#include "swaprandomvertexins_test.h"
#include "../vertex_graph.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>

QString SwapRandomVertexIns_Test::name(){
	return "SwapRandomVertexIns_Test";
};

void SwapRandomVertexIns_Test::printStat(int i, int cicles, int max){
	std::cout << "\r\t Calculates " << i << " (cicles: " << cicles << " / " << max << ") ";
}

bool SwapRandomVertexIns_Test::run(){
	VertexGraph vg(128);
	vg.genBase();
	int nMax = 1000;
	int cicles = 0;
	std::cout << "\n";
	printStat(0, cicles, nMax);
	
	for(int i = 0; i < nMax; i++){
		printStat(i, cicles, nMax);
		VertexGraph *pVertexGraphClone = vg.clone();
		pVertexGraphClone->swapRandomVertextIns();
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

