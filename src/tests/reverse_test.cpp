#include "reverse_test.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>

QString Reverse_Test::name(){
	return "Reverse_Test";
};

bool Reverse_Test::run(){
    QVector<QString> tests;
    tests.push_back("123");
    tests.push_back("66^lvp/-!A");
    tests.push_back("YN!KAHfPjFU[\">IwHL");
    tests.push_back("admin");
    tests.push_back("adminadmin");
    tests.push_back("test");
    tests.push_back("test1234");
    tests.push_back("1234");


    /*QMap<QString, QString> tests;
	tests["202CB962AC59075B964B07152D234B70"] = "123";
	tests["92BA7B54A295FED5060A2BA44A72E595"] = "66^lvp/-!A";
	tests["b885ca6d1c4e8231928ef2af5737426c"] = "YN!KAHfPjFU[\">IwHL";
	tests["21232f297a57a5a743894a0e4a801fc3"] = "admin";
	tests["f6fdffe48c908deb0f4c3bd36c032e72"] = "adminadmin";
	tests["098f6bcd4621d373cade4e832627b4f6"] = "test";
	tests["16d7a4fca7442dda3ad93c9a726597e4"] = "test1234";
    tests["81dc9bdb52d04dc20036dbd8313ed055"] = "1234";*/
	
	
    RHMemory *pMemory = new RHMemory();
    pMemory->dataFrom(tests);


	/*reverse_hash::Memory *pMemory = new reverse_hash::Memory();
	pMemory->load("md5/memory_md5_10000.rhmem");
	reverse_hash::MemoryItem memoryItem = pMemory->at(1);
	tests[memoryItem.output.toHex()] = memoryItem.input.toHex();*/
	
    /*foreach(QString key, tests.keys()){
		QVector<bool> vInput;
		reverse_hash::convertHEXStringToVBool(key, vInput, 128);
		
		QVector<bool> vOutputExpected;
		QByteArray originalString = tests.value(key).toUtf8();
		reverse_hash::convertArrayToVBool(originalString, vOutputExpected, originalString.size()*8);
	 
		int nCount = 55*8;
		QVector<bool> vOutput;
		for (int i = 0; i < nCount; i++) {
			QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			QFile file(filename);
			if(file.exists()){
				VertexGraph pVertexGraph(128);
				pVertexGraph.loadFromFile(filename);
				pVertexGraph.setIn(vInput);
				vOutput.push_back(pVertexGraph.out());
			}else{
				std::cerr << "Error: File '" << filename.toStdString() << "'does not exists\n";
			}
		}

		int min = std::min(vOutputExpected.size(), vOutput.size());
		int nCheck = 0;
		for(int i = 0; i < min; i++){
			if(vOutput[i] == vOutputExpected[i])
				nCheck++;
		}
		for(int i = min; i < vOutput.size(); i++ ){
			if(vOutput[i] == false)
				nCheck++;
		}
		int  nPersent = (nCheck * 100) / nCount;
		std::cout << "\t " << key.toStdString() << " => " << tests.value(key).toStdString() << "; reverted " << nPersent << "% [" << nCheck << "/" << nCount << " bits]\n";
    }*/
	return false;
};

