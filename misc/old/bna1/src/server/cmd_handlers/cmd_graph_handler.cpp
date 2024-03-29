#include <cmd_graph_handler.h>
#include <memory.h>
#include <QFile>
#include <QJsonArray>
#include <QFileInfo>
#include <bna.h>

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
	for (int nBitid = 0; nBitid < 440; nBitid++) {
		QString tmpBitid = QString::number(nBitid).rightJustified(3, '0');
		if(tmpBitid == bitid){
			bInvalid = false;
		}
	}

	if(bInvalid){
		pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(400, "Invalid value for 'bitid'"));
		return;
	}

	QString name = bitid;
	QString subdir = name[0] + "/" + name[1] + "/" + name[2];
	QString filename_statistics = "/usr/share/reversehashd/md5/" + subdir + "/" + name + ".statistics";
	QString filename_bna = "/usr/share/reversehashd/md5/" + subdir + "/" + name + ".bna";

	BinaryNeuralAcid bna;
	if(!bna.load(filename_bna)){
		pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(500,  "File '" + filename_bna + "'does not exists"));	
	}
	jsonData["graph"] = bna.toJson();
	pReverseHashDServer->sendMessage(pClient, jsonData);
}
