#ifndef CMD_GRAPH_HANDLER_H
#define CMD_GRAPH_HANDLER_H

#include <icmdhandler.h>
#include <ireversehashdserver.h>

#include <QString>
#include <QVariant>

class CmdGraphHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject obj);
};

#endif // CMD_GRAPH_HANDLER_H
