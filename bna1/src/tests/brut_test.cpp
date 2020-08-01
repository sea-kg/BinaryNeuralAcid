#include "brut_test.h"
#include <iostream>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QMap>
#include <QDir>
#include <QByteArray>
#include <QCryptographicHash>
#include <bna.h>

QString Brut_Test::name(){
	return "Reverse_Test";
}

struct Reverse_Test_Struct{
    Reverse_Test_Struct(QString md5, QString text) : md5(md5), text(text) {}
    QString md5;
    QString text;
};

bool Brut_Test::run(){
	
	QString sOrigHash = "81dc9bdb52d04dc20036dbd8313ed055";
    // QString sReverseHash0 = "c19173c3d173cc7e07cf5f19c28d0083";
    QString sReverseText = "6467756476606062606262606000404000000000000000001000000000220000000000002080000000000002002000004200140202515b";
    QByteArray text =  QByteArray::fromHex(sReverseText.toLatin1());
    // text = QString(text).toLatin1();

    QByteArray md5Text = QCryptographicHash::hash(text, QCryptographicHash::Md5);
    QString sMd5Text = QString(md5Text.toHex());

    int nPrevResult = BNACalculateBinDistance(BNAConvertHexToBin(sOrigHash), BNAConvertHexToBin(sMd5Text));
    std::cout << "Start: " << nPrevResult << "\n";

    // bool
    bool bExists = true;
    while(bExists){
        bExists = false;
        QByteArray textNew = BNATryBrutFast1(text, sOrigHash);
        textNew = BNATryBrutFast2(textNew, sOrigHash);

        QByteArray md5TextNew = QCryptographicHash::hash(textNew, QCryptographicHash::Md5);
        QString sMd5TextNew = QString(md5TextNew.toHex());

        QString sText_bin = BNAConvertHexToBin(sOrigHash);
        QString sTextNew_bin = BNAConvertHexToBin(sMd5TextNew);

        int nNewResult = BNACalculateBinDistance(sText_bin, sTextNew_bin);
        if(nNewResult > nPrevResult){
            bExists = true;
            text = QByteArray(textNew);
            nPrevResult = nNewResult;
            std::cout << "Distance: " << nPrevResult << "\n";
        }
    }

    text = QString(text).toLatin1();
    md5Text = QCryptographicHash::hash(text, QCryptographicHash::Md5);
    sMd5Text = QString(md5Text.toHex());

    nPrevResult = BNACalculateBinDistance(BNAConvertHexToBin(sOrigHash), BNAConvertHexToBin(sMd5Text));
    std::cout << "Start: " << nPrevResult << "\n";

    bExists = true;
    while(bExists){
        bExists = false;
        QByteArray textNew = BNATryBrutFast1(text, sOrigHash);
        textNew = BNATryBrutFast2(textNew, sOrigHash);

        QByteArray md5TextNew = QCryptographicHash::hash(textNew, QCryptographicHash::Md5);
        QString sMd5TextNew = QString(md5TextNew.toHex());

        QString sText_bin = BNAConvertHexToBin(sOrigHash);
        QString sTextNew_bin = BNAConvertHexToBin(sMd5TextNew);

        int nNewResult = BNACalculateBinDistance(sText_bin, sTextNew_bin);
        if(nNewResult > nPrevResult){
            bExists = true;
            text = QByteArray(textNew);
            nPrevResult = nNewResult;
            std::cout << "Distance: " << nPrevResult << "\n";
        }
    }

    std::cout << "Original: " << sReverseText.toStdString() << "\n";
    std::cout << "Reverted: " << QString(text.toHex()).toStdString() << "\n";

	return false;
}

