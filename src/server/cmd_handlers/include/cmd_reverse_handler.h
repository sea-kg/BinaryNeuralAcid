#ifndef CMD_REVERSE_HANDLER_H
#define CMD_REVERSE_HANDLER_H

#include <icmdhandler.h>
#include <ireversehashdserver.h>

#include <QString>
#include <QVariant>

class CmdReverseHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject obj);
};

#endif // CMD_REVERSE_HANDLER_H
