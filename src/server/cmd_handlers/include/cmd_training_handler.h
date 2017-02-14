#ifndef CMD_TRAINING_HANDLER_H
#define CMD_TRAINING_HANDLER_H

#include "../../../interfaces/icmdhandler.h"
#include "../../../interfaces/iwebsocketserver.h"

#include <QString>
#include <QVariant>

class CmdTrainingHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj);
};

#endif // CMD_TRAINING_HANDLER_H
