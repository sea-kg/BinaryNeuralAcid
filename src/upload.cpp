#include "sinchronizationdatafiles.h"
#include <iostream>
#include <cstring>
#include <QDataStream>
#include <QFile>
#include <QCryptographicHash>

namespace reverse_hash {
    SyncronizatonData::SyncronizatonData(){
    }
    
    void SyncronizatonDataFiles::upload(){
		// TODO read config
		QUrl testUrl("http://localhost/upload/test.php");
		QNetworkRequest request(testUrl);
		QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	}
}
