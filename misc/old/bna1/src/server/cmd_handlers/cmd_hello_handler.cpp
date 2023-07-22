#include "include/cmd_hello_handler.h"

QString CmdHelloHandler::cmd(){
	return "hello";
}

void CmdHelloHandler::handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject req){
	int rid = 0;
	if(req.contains("id")){
		rid = req["id"].toInt();
	}
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["rid"] = rid;
	pReverseHashDServer->sendMessage(pClient, jsonData);
}
