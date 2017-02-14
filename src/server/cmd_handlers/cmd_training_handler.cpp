#include "include/cmd_statistics_handler.h"
#include "../../vertex_graph.h"
#include "../../helpers.h"
#include "../../memory.h"
#include "../../memoryitem.h"
#include <QFile>
#include <QJsonArray>
#include <QFileInfo>

QString CmdStatisticsHandler::cmd(){
	return "statistics";
}

void CmdStatisticsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject req){
	int rid = 0;
	if(req.contains("id")){
		rid = req["id"].toInt();
	}
	
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["rid"] = rid;
	
	QJsonArray statistics;
	int nCount = 55*8;
	for (int i = 0; i < nCount; i++) {
		QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
		QFile file(filename);
		QFileInfo fi(filename);
		if(file.exists()){
			reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
			pVertexGraph->loadFromFile(filename);
			QJsonObject sbit;
			sbit["modified"] = fi.lastModified().toString("yyyy-MM-dd hh-mm-ss (t)");
			sbit["name"] = "bit" + QString::number(i).rightJustified(3, '0');
			sbit["lp"] = pVertexGraph->lastSuccessPersents();
			statistics.append(sbit);
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), rid, Error(500,  "File '" + filename + "'does not exists"));
			return;
		}
	}
	jsonData["statistics"] = statistics;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
