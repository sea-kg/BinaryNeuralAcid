#include "training_thread.h"

TrainingThread::TrainingThread(IWebSocketServer *pWebSocketServer){
	m_pWebSocketServer = pWebSocketServer;
}

void TrainingThread::run(){
	qDebug() << "Start Training Thread";
	while(true){
		qDebug() << "work";
		QJsonObject jsonData;
		jsonData["cmd"] = QJsonValue("training");
		jsonData["rid"] = QJsonValue(0);
		jsonData["result"] = "OK";
		jsonData["status"] = "work";
		m_pWebSocketServer->sendToAll(jsonData);
		QThread::sleep(5);
		
	}
	qDebug() << "Stop Training Thread";
}
