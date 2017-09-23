#include <cmd_graph_handler.h>
#include <vertex_graph.h>
#include <helpers.h>
#include <memory.h>
#include <memoryitem.h>
#include <QFile>
#include <QJsonArray>
#include <QFileInfo>

QString CmdGraphHandler::cmd(){
	return "graph";
}

void CmdGraphHandler::handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject req){
	int rid = 0;
	if(req.contains("id")){
		rid = req["id"].toInt();
	}
	
	if(!req.contains("bitid")){
		pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(400, "Not found parameter 'bitid'"));
		return;
	}
	
	
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["rid"] = rid;
	
	
	QString bitid = req["bitid"].toString();
	bool bInvalid = true;
	int nCount = 55*8;
	for (int i = 0; i < nCount; i++) {
		QString tmpBitid = "bit" + QString::number(i).rightJustified(3, '0');
		if(tmpBitid == bitid){
			bInvalid = false;
		}
	}

	if(bInvalid){
		pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(400, "Invalid value for 'bitid'"));
		return;
	}

	QString filename = "/usr/share/reversehashd/md5/" + bitid + ".vertexgraph";
	QFile file(filename);
	QFileInfo fi(filename);
	if(file.exists()){
		VertexGraph vg(128);
		vg.loadFromFile(filename);
		jsonData["graph"] = vg.conv2json();
	}else{
		pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(500,  "File '" + filename + "'does not exists"));
		return;
	}
	pReverseHashDServer->sendMessage(pClient, jsonData);
}
