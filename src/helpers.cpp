#include "helpers.h"
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QDebug>
#include <QDataStream>
#include <QCryptographicHash>

namespace reverse_hash {

    // ----------------------------------------------------------------

	QString convertCharToHexCode(unsigned char c) {
		if(c == 0) return "0";
		if(c == 1) return "1";
		if(c == 2) return "2";
		if(c == 3) return "3";
		if(c == 4) return "4";
		if(c == 5) return "5";
		if(c == 6) return "6";
		if(c == 7) return "7";
		if(c == 8) return "8";
		if(c == 9) return "9";
		if(c == 10) return "a";
		if(c == 11) return "b";
		if(c == 12) return "c";
		if(c == 13) return "d";
		if(c == 14) return "e";
		if(c == 15) return "f";
		return "x";
	}

	// ----------------------------------------------------------------

    QString convertVBoolHEXString(QVector<bool> &vars) {
		QString result = "";
		unsigned char c = 0;
		for(int i = 0; i < vars.size(); i++){
			if(i % 4 == 0 && i != 0){
				result += convertCharToHexCode(c);
				c = 0x00;
			}
			c = (c << 1) | (vars[i] ? 0x01 : 0x00);
		}
		result += convertCharToHexCode(c);
        return result;
    }
}

int loadPersent(QString filename){
	int nPersent = 0;
	// load persent
	QFile file(filename);
	if (!file.exists()) {
		qDebug().noquote().nospace() << "Helpers/loadPersent:  File did not exists: " << filename;
	}else{
		if (!file.open(QIODevice::ReadOnly) ) {
			qDebug().noquote().nospace() << "Helpers/loadPersent: Could not open file " << filename;
		}else{
			QDataStream stream(&file);
			stream >> nPersent;
		}
	}
	return nPersent;
}

void savePersent(QString filename, int nPersent){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Helpers/savePersent: Could not write file: " << filename;
		return;
	}
	QDataStream stream( &file );
	stream << nPersent;
	file.close();
}

QString prepareName(int bitid){
    return QString::number(bitid).rightJustified(3, '0');
}

QString prepareSubdir(int bitid){
    QString m_sBitid = QString::number(bitid).rightJustified(3, '0');
    return m_sBitid[0] + "/" + m_sBitid[1] + "/" + m_sBitid[2];
}
