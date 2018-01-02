#include "learning_test.h"
#include "../helpers.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>
#include <QDir>
#include <QThread>
#include <bna.h>

QString Learning_Test::name(){
    return "Learning_Test";
}

int calculate(QMap<QString, QString> &tests, int bitid, BNA &bna){
    int nResult = 0;
    foreach(QString key, tests.keys()){
        QVector<bool> vInput;
        reverse_hash::convertHEXStringToVBool(key, vInput, 128);

        QVector<bool> vOutputExpected;
        QByteArray originalString = tests.value(key).toUtf8();
        reverse_hash::convertArrayToVBool(originalString, vOutputExpected, originalString.size()*8);

        bool bResult = bna.calc(vInput, 0);
        if(vOutputExpected[bitid] == bResult){
            nResult++;
        }
    }
    return nResult;
}

bool Learning_Test::run(){

    QMap<QString, QString> tests;
	tests["202CB962AC59075B964B07152D234B70"] = "123";
	tests["92BA7B54A295FED5060A2BA44A72E595"] = "66^lvp/-!A";
	tests["b885ca6d1c4e8231928ef2af5737426c"] = "YN!KAHfPjFU[\">IwHL";
	tests["21232f297a57a5a743894a0e4a801fc3"] = "admin";
	tests["f6fdffe48c908deb0f4c3bd36c032e72"] = "adminadmin";
	tests["098f6bcd4621d373cade4e832627b4f6"] = "test";
	tests["16d7a4fca7442dda3ad93c9a726597e4"] = "test1234";
    tests["81dc9bdb52d04dc20036dbd8313ed055"] = "1234";

    int nCount = 55*8;
    for (int bitid = 0; bitid < nCount; bitid++) {
        QString name = QString::number(bitid).rightJustified(3, '0');
        QString subdir = name[0] + "/" + name[1] + "/" + name[2];
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
        int p = calculate(tests, bitid, bna);
        std::cout << m_sBitid.toStdString() << " learning start... " << p << "/" << tests.size() << "\n";
        if(p == tests.size()){
            continue;
        }
        int prev_v = p;
        int i = 0;
        QByteArray keep = bna.exportToByteArray();
        while(i < 1000){
            bna.generateRandomMutations(20);
            p = calculate(tests, bitid, bna);
            if(p > prev_v){
               bna.save(m_sFilename);
               prev_v = p;
               std::cout << m_sBitid.toStdString() << " Updated to " << p << "/" << tests.size() << "\n";
            }else{
                bna.importFromByteArray(keep);
            }
            if(prev_v == tests.size()){
                break;
            }
            i++;
            if(i%100 == 0){
                QThread::msleep(100);
            }
        }
    }
	return false;
}

