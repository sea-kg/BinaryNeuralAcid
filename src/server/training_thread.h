#ifndef TRINAING_THREAD_H
#define TRINAING_THREAD_H

#include <QCoreApplication>
#include <QString>
#include <QThread>
#include <QDebug>
#include "../interfaces/iwebsocketserver.h"

class TrainingThread : public QThread{
		Q_OBJECT
		IWebSocketServer *m_pWebSocketServer;
		int m_nSleep;
		void sendMessage(QString bitid, QString message);
	public:
		TrainingThread(IWebSocketServer *pWebSocketServer);
	protected:
		void run();
};

#endif // TRINAING_THREAD_H
