#include "reverse_test.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>
#include <QDir>
#include <bna.h>

QString Reverse_Test::name(){
	return "Reverse_Test";
};

bool Reverse_Test::run(){

    QMap<QString, QString> tests;
	tests["202CB962AC59075B964B07152D234B70"] = "123";
	tests["92BA7B54A295FED5060A2BA44A72E595"] = "66^lvp/-!A";
	tests["b885ca6d1c4e8231928ef2af5737426c"] = "YN!KAHfPjFU[\">IwHL";
	tests["21232f297a57a5a743894a0e4a801fc3"] = "admin";
	tests["f6fdffe48c908deb0f4c3bd36c032e72"] = "adminadmin";
	tests["098f6bcd4621d373cade4e832627b4f6"] = "test";
	tests["16d7a4fca7442dda3ad93c9a726597e4"] = "test1234";
    tests["81dc9bdb52d04dc20036dbd8313ed055"] = "1234";
    tests["b7608279dd8566a122364ea6eb44edc4"] = "VeryHardPassword";

    foreach(QString key, tests.keys()){
        QVector<BNABit> vInput;
        BNAConvertHEXStringToVBool(key, vInput, 128);
		
        QVector<BNABit> vOutputExpected;
		QByteArray originalString = tests.value(key).toUtf8();
        BNAConvertArrayToVBool(originalString, vOutputExpected, 55*8);
	 
		int nCount = 55*8;
        QVector<BNABit> vOutput;
        for (int bitid = 0; bitid < nCount; bitid++) {
            QString name = prepareName(bitid);
            QString subdir = prepareSubdir(bitid);
            QString m_sBitid = name;
            QString m_sDir = "tests_bna_md5/" + subdir;
            QString m_sFilename = m_sDir + "/" + name + ".bna";
            QFile file(m_sFilename);
            QDir dir(".");
            dir.mkpath(m_sDir);
            BNA bna;
            if(!file.exists()){
                std::cerr << "Error: File '" << m_sFilename.toStdString() << "'does not exists\n";
                bna.randomGenerate(128,1,100);
                bna.save(m_sFilename);
            }else{
                bna.load(m_sFilename);
            }
            BNABit bResult = bna.calc(vInput, 0);
            vOutput.push_back(bResult);
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
    }
	return false;
}

