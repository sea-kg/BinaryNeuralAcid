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
	VertexGraph vg(128);
	vg.genBase();
	for(int i = 0; i < 1000; i++){
		vg.changeRandomOperation();
		vg.out();
	}
	return true;
};

