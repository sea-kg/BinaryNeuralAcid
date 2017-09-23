#ifndef INTERFACES_ICMDHANDLER_H
#define INTERFACES_ICMDHANDLER_H

#include <QString>
#include <QJsonObject>
#include "ireversehashdserver.h"

class ICmdHandler {
	public:
		virtual QString cmd() = 0;
		virtual void handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject obj) = 0;
};

#endif // INTERFACES_ICMDHANDLER_H
