#include "changerandomoperation_test.h"
#include "../vertex_graph.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>

QString ChangeRandomOperation_Test::name(){
	return "ChangeRandomOperation_Test";
};

bool ChangeRandomOperation_Test::run(){
	reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
	pVertexGraph->genBase();
	for(int i = 0; i < 1000; i++){
		pVertexGraph->changeRandomOperation();
		pVertexGraph->out();
	}
	return true;
};

