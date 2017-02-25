#ifndef IWEBSOCKETSERVER_H
#define IWEBSOCKETSERVER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

#include "../server/error.h"
 
class IWebSocketServer {
	public:
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj) = 0;
		virtual void sendMessageError(QWebSocket *pClient, QString cmd, int id, Error obj) = 0;
		virtual void sendToAll(QJsonObject obj) = 0;
};

#endif // IWEBSOCKETSERVER_H
