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

static QMap<QChar, QString> gMapHEX2BIN;

void initMapHEX2BIN(){
	if(gMapHEX2BIN.size() == 0){
		gMapHEX2BIN['0'] = "0000";
		gMapHEX2BIN['1'] = "0001";
		gMapHEX2BIN['2'] = "0010";
		gMapHEX2BIN['3'] = "0011";
		gMapHEX2BIN['4'] = "0100";
		gMapHEX2BIN['5'] = "0101";
		gMapHEX2BIN['6'] = "0110";
		gMapHEX2BIN['7'] = "0111";
		gMapHEX2BIN['8'] = "1000";
		gMapHEX2BIN['9'] = "1001";
		gMapHEX2BIN['A'] = "1010";
		gMapHEX2BIN['B'] = "1011";
		gMapHEX2BIN['C'] = "1100";
		gMapHEX2BIN['D'] = "1101";
		gMapHEX2BIN['E'] = "1110";
		gMapHEX2BIN['F'] = "1111";
		gMapHEX2BIN['a'] = gMapHEX2BIN['A'];
		gMapHEX2BIN['b'] = gMapHEX2BIN['B'];
		gMapHEX2BIN['c'] = gMapHEX2BIN['C'];
		gMapHEX2BIN['d'] = gMapHEX2BIN['D'];
		gMapHEX2BIN['e'] = gMapHEX2BIN['E'];
		gMapHEX2BIN['f'] = gMapHEX2BIN['F'];
	}
};

QString hex2bin(QString sHex){
	initMapHEX2BIN();
	QString sResult = "";
	for (int i = 0; i < sHex.length(); i++){
		QChar ch = sHex.at(i);
		if(gMapHEX2BIN.contains(ch)){
			sResult += gMapHEX2BIN[ch];
		}else{
			std::cerr << "[ERROR] Unknown hex char\n";
		}
	}
	return sResult;
};

QString bin2hex(QString sBin) {
    QString result = "";
    unsigned char c = 0;
    for(int i = 0; i < sBin.length(); i++){
		QChar ch = sBin.at(i);
        if(i % 4 == 0 && i != 0){
            result += BNAConvertCharToHexCode(c);
            c = 0x00;
        }
        c = (c << 1) | (ch == '1' ? 0x01 : 0x00);
    }
    result += BNAConvertCharToHexCode(c);
    return result;
}

int calcDistance(QString sBin1, QString sBin2){
	if(sBin1.length() != sBin2.length()){
		std::cerr << "[ERROR] Different length \n";
		return 1000;
	}

    if(sBin1.length() == 0 || sBin2.length() == 0){
        std::cerr << "[ERROR] Length is 0 - wrong \n";
        return 1000;
    }
	int nDistance = 0;
	for (int i = 0; i < sBin1.length(); i++){
		QChar ch1 = sBin1.at(i);
		QChar ch2 = sBin2.at(i);
		if(ch1 != ch2){
			nDistance++;
		}
	}
	return nDistance;
};

bool Brut_Test::run(){
	initMapHEX2BIN();
	
	QString sOrigHash = "81dc9bdb52d04dc20036dbd8313ed055";
	QString sReverseHash0 = "dc1aa77aa041a51259802678f76871e8";
	QString sReverseText = "6467756476606060606062606000404000400002000000001008000082280000402001000880000000201cc2006101004200140202515b";
	
	QByteArray text =  QByteArray::fromHex(sReverseText.toLatin1());
	
	QByteArray result_md5 = QCryptographicHash::hash(text, QCryptographicHash::Md5);
	QString sReverseHash1 = QString(result_md5.toHex());
	if(sReverseHash0 != sReverseHash1){
		std::cout << "[ERROR] hash not equals \n";
		return false;
	}
	
	std::cout << "Hash started: " << sReverseHash1.toStdString() << "\n";
	
	// erase text
	QString text2 = QString::fromUtf8(text.data());
	std::cout << "Erased text: " << text2.toStdString() << "\n";
	text = QString(text2).toLatin1();
	QByteArray md5_text = QCryptographicHash::hash(text, QCryptographicHash::Md5);
	QString sReverseHash2 = QString(md5_text.toHex());
	std::cout << "Hash erased: " << sReverseHash2.toStdString() << "\n";
	QString sReverseHash3 = "8c3bf847e9aa5ddf08f6a1a48f857034";
	
	if(sReverseHash2 != sReverseHash3){
		std::cout << "[ERROR] erased hash not equals \n";
		std::cout << "	-> Expected " << sReverseHash3.toStdString() << "\n";
		std::cout << "	-> Got      " << sReverseHash2.toStdString() << "\n";
		return false;
	}
	QString sOrigHash_bin = hex2bin(sOrigHash);
	QString sReverseHash1_bin = hex2bin(sReverseHash1);
	QString sReverseHash2_bin = hex2bin(sReverseHash2);
	std::cout << "Hash orig:    " << sOrigHash_bin.toStdString() << "\n";
	std::cout << "Hash started: " << sReverseHash1_bin.toStdString() << "\n";
	std::cout << "Hash erased:  " << sReverseHash2_bin.toStdString() << "\n";
	// std::cout << "Hash erased:  " << bin2hex(sReverseHash2_bin).toStdString() << "\n";
	
	// std::cout << "Distance 1-2 :    " << calcDistance(sReverseHash1_bin, sReverseHash2_bin) << "\n";
	// std::cout << "Distance 1-orig:  " << calcDistance(sReverseHash1_bin, sOrigHash_bin) << "\n";
	std::cout << "Distance 2-orig:  " << calcDistance(sReverseHash2_bin, sOrigHash_bin) << "\n";
	
	int nPrevDistance = calcDistance(sReverseHash2_bin, sOrigHash_bin);
    QString sPrevText = hex2bin(text2.toLatin1().toHex());
	std::cout << "Better text:      " << text2.toLatin1().toHex().toStdString() << "\n";
    std::cout << "Start Erased text:  " << sPrevText.toStdString() << "\n";
    bool bExists = true;
    int nRound = 0;
    while(bExists){
        bExists = false;
        std::cout << "Round #" << nRound << "\n";
        nRound++;
        QString sTmpPrevText = QString(sPrevText);
        for(int i = 0; i < sTmpPrevText.length(); i++){
            QChar ch = sTmpPrevText.at(i);
            ch = ch == '1' ? '0' : '1';
            sTmpPrevText[i] = ch;
            // std::cout << "Erased text:  " << text3.toStdString() << "\n";
            QString sTmpHex = bin2hex(sTmpPrevText);
            QByteArray bTmpText =  QByteArray::fromHex(sTmpHex.toLatin1());
            QByteArray sTmpMD5 = QCryptographicHash::hash(bTmpText, QCryptographicHash::Md5);
            QString sTmpMD5_hex = QString(sTmpMD5.toHex());
            QString sTmpMD5_bin = hex2bin(sTmpMD5_hex);
            int nNewDistance = calcDistance(sOrigHash_bin, sTmpMD5_bin);
            if(nNewDistance < nPrevDistance){
                sPrevText = QString(sTmpPrevText);
                std::cout << "Distance:  " << nNewDistance << "\n";
                nPrevDistance = nNewDistance;
                if(nNewDistance == 0){
                    std::cout << "Distance:  " << sTmpPrevText.toStdString() << "\n";
                    std::cout << bin2hex(sTmpPrevText).toStdString() << "\n";

                }
                bExists = true;
            }
            ch = ch == '1' ? '0' : '1';
            sTmpPrevText[i] = ch;
        }
        std::cout << "Better distance:  " << nPrevDistance << "\n";
        std::cout << "Better text:        " << sPrevText.toStdString() << "\n";
        std::cout << "Better text:      " << bin2hex(sPrevText).toStdString() << "\n";
    }

    QByteArray bTmpText =  QByteArray::fromHex(bin2hex(sPrevText).toLatin1());
    QByteArray sTmpMD5 = QCryptographicHash::hash(bTmpText, QCryptographicHash::Md5);
    QString sTmpMD5_hex = QString(sTmpMD5.toHex());

    std::cout << " ==> Found better result\n";
    std::cout << "     Distance to original:  " << nPrevDistance << "\n";
    std::cout << "     Incoming text:         " << sPrevText.toStdString() << "\n";
    std::cout << "     Got MD5:               " << sTmpMD5_hex.toStdString() << "\n";
    std::cout << "     Expected Hash:         " << sOrigHash.toStdString() << "\n";

	
	/*
	 * 
	 * 1. Отсеять все что после 0 байта
2. Получить процент восстановления бита
3. Отсортировать по процентам восстановления
4. Изменить самый с наименьшим процентом
5. Посчитать хеш если хеш
6. Сравнить с оригинальным хешем (если он стал ближе - количество бит совпадает больше) то значит что то улушилось и перейти к 1 шагу
7. Вернуть значение бита и перейти к следующему
* 
	 * */
	
	// std::cout << str2.toStdString() << "\n";
	
	return false;
}

