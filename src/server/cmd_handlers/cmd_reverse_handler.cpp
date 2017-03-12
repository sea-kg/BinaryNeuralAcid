#include "include/cmd_reverse_handler.h"
#include "../../vertex_graph.h"
#include "../../helpers.h"
#include "../../memory.h"
#include "../../memoryitem.h"
#include <QFile>
#include <QJsonDocument>

QString CmdReverseHandler::cmd(){
	return "reverse";
}

void CmdReverseHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject req){
	int rid = 0;
	if(req.contains("id")){
		rid = req["id"].toInt();
	}
	
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["rid"] = rid;
	
	if(!req.contains("md5")){
		pWebSocketServer->sendMessageError(pClient, cmd(), rid, Error(400, "Not found parameter 'md5'"));
		return;
	}
	
	QString hash = req["md5"].toString();
	QVector<bool> vOutput;
	QVector<bool> vInput;
	QString answer_bin = "";
	reverse_hash::convertHEXStringToVBool(hash, vInput, 128);
	int nCount = 55*8;
	for (int i = 0; i < nCount; i++) {
		bool bResult = false;
		QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
		QFile file(filename);
		if(file.exists()){
			VertexGraph vg(128);
			vg.loadFromFile(filename);
			vg.setIn(vInput);
			bResult = vg.out();
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), rid, Error(500,  "File '" + filename + "'does not exists"));
			return;
		}
		vOutput.push_back(bResult);
		answer_bin += (bResult ? "1" : "0");
	}
	jsonData["output_count"] = vOutput.size();
	QString answer_hex = reverse_hash::convertVBoolHEXString(vOutput);
	QByteArray text = QByteArray::fromHex(answer_hex.toLatin1());
	jsonData["answer_bin"] = answer_bin;
	jsonData["answer_text"] = QString(text);
	jsonData["answer_hex"] = answer_hex;
	jsonData["answer_base64"] = QString(text.toBase64());
	jsonData["request_md5"] = hash; // TODO check comeback reverse
	QByteArray result_md5 = QCryptographicHash::hash(text, QCryptographicHash::Md5);
	jsonData["result_md5"] = QString(result_md5.toHex());

	QJsonDocument doc(jsonData);
	QString message = doc.toJson(QJsonDocument::Compact);
	qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] >>> " << message;
		
	pWebSocketServer->sendMessage(pClient, jsonData);
}
