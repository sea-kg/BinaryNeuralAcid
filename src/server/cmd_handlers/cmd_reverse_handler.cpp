#include <cmd_reverse_handler.h>
#include <bna.h>
#include <helpers.h>
#include <memory.h>
#include <QFile>
#include <QJsonDocument>

QString CmdReverseHandler::cmd(){
	return "reverse";
}

void CmdReverseHandler::handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject req){
	int rid = 0;
	if(req.contains("id")){
		rid = req["id"].toInt();
	}

    QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["rid"] = rid;
	
	if(!req.contains("md5")){
		pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(400, "Not found parameter 'md5'"));
		return;
	}
	
	QString hash = req["md5"].toString();
    QVector<BNABit> vOutput;
    QVector<BNABit> vInputs;
	QString answer_bin = "";
    BNAConvertHEXStringToVBool(hash, vInputs, 128);
	int nCount = 55*8;
    for (int bitid = 0; bitid < nCount; bitid++) {
        BNABit bResult = B_0;
        QString name = prepareName(bitid);
        QString subdir = prepareSubdir(bitid);
        QString m_sBitid = name;
        QString m_sDir = "tests_bna_md5/" + subdir;
        QString m_sFilename = m_sDir + "/" + name + ".bna";

        QFile file(m_sFilename);
		if(file.exists()){
			BNA bna;
            bna.load(m_sFilename);
            bResult = bna.calc(vInputs, 0);
		}else{
            pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(500,  "File '" + m_sFilename + "'does not exists"));
			return;
		}
		vOutput.push_back(bResult);
        answer_bin += (bResult == B_1 ? "1" : "0");
	}
	jsonData["output_count"] = vOutput.size();
    QString answer_hex = BNAConvertVBoolHEXString(vOutput);
	QByteArray text = QByteArray::fromHex(answer_hex.toLatin1());
	jsonData["answer_bin"] = answer_bin;
	jsonData["answer_text"] = QString(text);
	jsonData["answer_hex"] = answer_hex;
	jsonData["answer_base64"] = QString(text.toBase64());
	jsonData["request_md5"] = hash; // TODO check comeback reverse
    QByteArray result_md5 = QCryptographicHash::hash(QString(text).toUtf8(), QCryptographicHash::Md5);
	jsonData["result_md5"] = QString(result_md5.toHex());

	QJsonDocument doc(jsonData);
	QString message = doc.toJson(QJsonDocument::Compact);
	qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] >>> " << message;
		
	pReverseHashDServer->sendMessage(pClient, jsonData);
}
