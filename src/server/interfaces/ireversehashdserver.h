#ifndef IREVERSEHASHDSERVER_H
#define IREVERSEHASHDSERVER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

#include "../server/error.h"
 
class IReverseHashDServer {
	public:
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj) = 0;
		virtual void sendMessageError(QWebSocket *pClient, QString cmd, int id, Error obj) = 0;
		virtual void sendToAll(QJsonObject obj) = 0;
};

#endif // IREVERSEHASHDSERVER_H
