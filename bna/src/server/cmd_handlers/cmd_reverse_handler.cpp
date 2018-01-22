#include <cmd_reverse_handler.h>
#include <bna.h>
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
    BNAProject *pBNAProject = pReverseHashDServer->getBNAProject();
    int nCount = pBNAProject->getOutputBits();
    for (int bitid = 0; bitid < nCount; bitid++) {
        BNABit bResult = B_0;
        BNA *pBNA = pBNAProject->getBNA(bitid);
        bResult = pBNA->calc(vInputs, 0);
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
