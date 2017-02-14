#ifndef CMD_STATISTICS_HANDLER_H
#define CMD_STATISTICS_HANDLER_H

#include "../../../interfaces/icmdhandler.h"
#include "../../../interfaces/iwebsocketserver.h"

#include <QString>
#include <QVariant>

class CmdStatisticsHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj);
};

#endif // CMD_STATISTICS_HANDLER_H
