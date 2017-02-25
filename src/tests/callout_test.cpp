#include "callout_test.h"
#include "../vertex_graph.h"
#include <iostream>

QString CallOut_Test::name(){
	return "CallOut_Test";
};

bool CallOut_Test::run(){
	VertexGraph pVertexGraph(128);
	pVertexGraph.genBase();
	pVertexGraph.out();
	pVertexGraph.randomChanges(50);
	pVertexGraph.out();
	return true;
};

