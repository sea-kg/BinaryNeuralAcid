#include <cmd_statistics_handler.h>
#include <bna.h>
#include <memory.h>
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

    BNAProject *pBNAProject = pReverseHashDServer->getBNAProject();
    QMap<int, int> mapResults = pBNAProject->getResults();
	QJsonArray statistics;
    int nCount = pBNAProject->getOutputBits();
    for (int bitid = 0; bitid < nCount; bitid++) {
        QJsonObject sbit;
        sbit["id"] = bitid;
        sbit["result"] = mapResults[bitid];
        statistics.append(sbit);
    }
	jsonData["statistics"] = statistics;
	pReverseHashDServer->sendMessage(pClient, jsonData);
}
