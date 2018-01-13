#include "helpers.h"
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QDebug>
#include <QDataStream>
#include <QCryptographicHash>

namespace reverse_hash {


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
