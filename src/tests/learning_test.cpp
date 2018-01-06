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

struct LearnTestData{
    LearnTestData(QString hash, QString key){
        sHash = hash;
        BNAConvertHEXStringToVBool(sHash, vInput, 128);

        sKey = key;
        QByteArray originalString = sKey.toUtf8();
        BNAConvertArrayToVBool(originalString, vOutputExpected, 55*8);
    }
    QString sHash;
    QString sKey;
    QVector<BNABit> vInput;
    QVector<BNABit> vOutputExpected;
};

int calculate3(QVector<LearnTestData *> &testData, int bitid, BNA &bna){
    int nResult = 0;
    for(int i = 0; i < testData.size(); i++){
        LearnTestData *pData = testData[i];
        bool bResult = bna.calc(pData->vInput, 0);
        // std::cout << "3, vOutputExpected[" << bitid << "] " << pData->vOutputExpected[bitid] << ", bResult: " << bResult << "\n";
        if(pData->vOutputExpected[bitid] == bResult){
            nResult++;
        }
    }
    return nResult;
}

bool Learning_Test::run(){
    QVector<LearnTestData *> testData;
    testData.push_back(new LearnTestData("202CB962AC59075B964B07152D234B70", "123"));
    testData.push_back(new LearnTestData("92BA7B54A295FED5060A2BA44A72E595", "66^lvp/-!A"));
    testData.push_back(new LearnTestData("b885ca6d1c4e8231928ef2af5737426c", "YN!KAHfPjFU[\">IwHL"));
    testData.push_back(new LearnTestData("21232f297a57a5a743894a0e4a801fc3", "admin"));
    testData.push_back(new LearnTestData("f6fdffe48c908deb0f4c3bd36c032e72", "adminadmin"));
    testData.push_back(new LearnTestData("098f6bcd4621d373cade4e832627b4f6", "test"));
    testData.push_back(new LearnTestData("16d7a4fca7442dda3ad93c9a726597e4", "test1234"));
    testData.push_back(new LearnTestData("81dc9bdb52d04dc20036dbd8313ed055", "1234"));
    testData.push_back(new LearnTestData("827CCB0EEA8A706C4C34A16891F84E7B", "12345"));
    testData.push_back(new LearnTestData("b7608279dd8566a122364ea6eb44edc4", "VeryHardPassword"));

    int nCount = 55*8;
    // int nCount = 41;
    for (int bitid = 0; bitid < nCount; bitid++) {
        QString m_sBitid = prepareName(bitid);
        QString subdir = prepareSubdir(bitid);
        QString m_sDir = "tests_bna_md5/" + subdir;
        QString m_sFilename = m_sDir + "/" + m_sBitid + ".bna";
        QFile file(m_sFilename);
        QDir dir(".");
        dir.mkpath(m_sDir);
        BNA bna;
        if(!bna.load(m_sFilename)){
            std::cerr << "Error: File '" << m_sFilename.toStdString() << "'does not exists\n";
            bna.randomGenerate(128,1,100);
            bna.save(m_sFilename);
        }
        int p = calculate3(testData, bitid, bna);

        if(p == testData.size()){
            continue;
        }
        std::cout << m_sBitid.toStdString() << " learning start... 1: " << p << "/" << testData.size() << "\n";

        int prev_v = p;
        int i = 0;
        QByteArray keep = bna.exportToByteArray();
        while(i < 1000){
            bna.generateRandomMutations(20);
            p = calculate3(testData, bitid, bna);
            if(p > prev_v){
                keep = bna.exportToByteArray();
                bna.save(m_sFilename);
                prev_v = p;
                std::cout << m_sBitid.toStdString() << " Updated to " << p << "/" << testData.size() << "\n";
            }else{
                bna.importFromByteArray(keep);
                // bna.load(m_sFilename);
            }
            if(prev_v == testData.size()){
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

