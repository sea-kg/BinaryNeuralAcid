#ifndef CMD_REVERSE_HANDLER_H
#define CMD_REVERSE_HANDLER_H

#include "../../../interfaces/icmdhandler.h"
#include "../../../interfaces/iwebsocketserver.h"

#include <QString>
#include <QVariant>

class CmdReverseHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj);
};

#endif // CMD_REVERSE_HANDLER_H
