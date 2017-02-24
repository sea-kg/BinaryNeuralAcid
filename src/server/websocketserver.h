#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMap>
#include <QFile>
#include <QSettings>

#include "../interfaces/icmdhandler.h"
#include "../interfaces/iwebsocketserver.h"
#include "error.h"
#include "training_thread.h"

class WebSocketServer : public QObject, public IWebSocketServer {
	
	private:
		Q_OBJECT
	public:
		explicit WebSocketServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
		~WebSocketServer();

		// IWebSocketServer
		virtual int getConnectedUsers();
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj);
		virtual void sendMessageError(QWebSocket *pClient, QString cmd, int id, Error obj);
		virtual void sendToAll(QJsonObject obj);

	Q_SIGNALS:
		void closed();
		void sendToAllSignal(QJsonObject obj);

	private Q_SLOTS:
		void onNewConnection();
		void processTextMessage(QString message);
		void processBinaryMessage(QByteArray message);
		void socketDisconnected();
		void sendToAllSlot(QJsonObject obj);

	private:
		QString readStringFromSettings(QSettings &sett, QString settName, QString defaultValue);
		int readIntFromSettings(QSettings &sett, QString settName, int defaultValue);
	
		QWebSocketServer *m_pWebSocketServer;
		QList<QWebSocket *> m_clients;
		QMap<QString, ICmdHandler *> m_mapCmdHandlers;
		bool m_debug;
		
		// settings
		QString m_sFilename;
		QString m_sPassword;
		
		QString m_sEmail_smtphost;
		int m_nEmail_smtpport;
		QString m_sEmail_username;
		QString m_sEmail_password;
		TrainingThread *m_pTrainingThread;
};

#endif //WEBSOCKETSERVER_H
