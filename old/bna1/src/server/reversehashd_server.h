#ifndef REVERSEHASHD_SERVER_H
#define REVERSEHASHD_SERVER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMap>
#include <QFile>
#include <QSettings>

#include <icmdhandler.h>
#include <ireversehashdserver.h>
#include <error.h>
#include <bna.h>
#include <training_thread.h>

class ReverseHashDServer : public QObject, public IReverseHashDServer {
	
	private:
		Q_OBJECT
	public:
        explicit ReverseHashDServer(quint16 port, QObject *parent = Q_NULLPTR);
		~ReverseHashDServer();

		// IReverseHashDServer
		virtual int getConnectedUsers();
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj);
		virtual void sendMessageError(QWebSocket *pClient, QString cmd, int id, Error obj);
		virtual void sendToAll(QJsonObject obj);
        virtual BNAProject * getBNAProject();

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
		void extractFile(QString res_name, QString to_name);
	
	
		QWebSocketServer *m_pReverseHashDServer;
		QList<QWebSocket *> m_clients;
		QMap<QString, ICmdHandler *> m_mapCmdHandlers;
        TrainingThread *m_pTrainingThread;

        BNAProject *m_pBnaProject;
};

#endif //REVERSEHASHD_SERVER_H
