#ifndef CMD_STATISTICS_HANDLER_H
#define CMD_STATISTICS_HANDLER_H

#include <icmdhandler.h>
#include <ireversehashdserver.h>

#include <QString>
#include <QVariant>

class CmdStatisticsHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject obj);
};

#endif // CMD_STATISTICS_HANDLER_H
