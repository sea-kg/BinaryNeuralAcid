#ifndef CMD_HELLO_HANDLER_H
#define CMD_HELLO_HANDLER_H

#include <icmdhandler.h>
#include <ireversehashdserver.h>

#include <QString>
#include <QVariant>

class CmdHelloHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject obj);
};

#endif // CMD_HELLO_HANDLER_H
