#include "readwrite_test.h"
#include "../vertex_graph.h"
#include <iostream>
#include <QDir>
#include <QTemporaryFile>

QString ReadWrite_Test::name(){
	return "ReadWrite_Test";
};

bool ReadWrite_Test::run(){
	reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
	pVertexGraph->genBase();

	qint64 size1 = 0, size2 = 0, size3 = 0;

	QTemporaryFile file1;
	file1.setAutoRemove(true);
	if(file1.open()){
		file1.close();
		pVertexGraph->save(file1.fileName());
		size1 = QFileInfo(file1.fileName()).size();
		std::cout << "\t" << file1.fileName().toStdString() << " = " << int(size1) << " \n";
		pVertexGraph->load(file1.fileName());
	}
	
	QTemporaryFile file2;
	file2.setAutoRemove(true);
	if(file2.open()){
		file2.close();
		pVertexGraph->save(file2.fileName());
		size2 = QFileInfo(file2.fileName()).size();
		std::cout << "\t" << file2.fileName().toStdString() << " = " << int(size2) << " \n";
		pVertexGraph->load(file2.fileName());
	}
	
	QTemporaryFile file3;
	file3.setAutoRemove(true);
	if(file3.open()){
		file3.close();
		pVertexGraph->save(file3.fileName());
		// pVertexGraph->saveDot("test/test1.dot");
		size3 = QFileInfo(file3.fileName()).size();
		std::cout << "\t" << file3.fileName().toStdString() << " = " << int(size3) << " \n";
		pVertexGraph->load(file3.fileName());
	}
	
	if(size1 == 0 || size2 == 0 || size3 == 0){
		return false;
	}
	
	if(size1 != size2 || size2 != size3 || size1 != size3){
		return false;
	}
 
	return true;
};

