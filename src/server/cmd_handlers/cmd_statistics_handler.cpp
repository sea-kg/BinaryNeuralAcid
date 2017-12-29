#include <cmd_statistics_handler.h>
#include <bna.h>
#include <helpers.h>
#include <memory.h>
#include <memoryitem.h>
#include <QFile>
#include <QJsonArray>
#include <QFileInfo>

QString CmdStatisticsHandler::cmd(){
	return "statistics";
}

void CmdStatisticsHandler::handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject req){
	int rid = 0;
	if(req.contains("id")){
		rid = req["id"].toInt();
	}
	
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["rid"] = rid;
	
	QJsonArray statistics;
	for (int bitid = 0; bitid < 440; bitid++) {
		QString name = QString::number(bitid).rightJustified(3, '0');
		QString subdir = name[0] + "/" + name[1] + "/" + name[2];

		QString filename = "/usr/share/reversehashd/md5/" + subdir + "/" + name + ".statistics";
		QFile file(filename);
		QFileInfo fi(filename);
		if(file.exists()){
			QJsonObject sbit;
			sbit["modified"] = fi.lastModified().toString("yyyy-MM-dd hh-mm-ss (t)");
			sbit["name"] = "bit" + QString::number(bitid).rightJustified(3, '0');
			sbit["lp"] = loadPersent(filename);
			statistics.append(sbit);
		}else{
			pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(500,  "File '" + filename + "'does not exists"));
			return;
		}
	}
	jsonData["statistics"] = statistics;
	pReverseHashDServer->sendMessage(pClient, jsonData);
}
