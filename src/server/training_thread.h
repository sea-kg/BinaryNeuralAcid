#ifndef TRINAING_THREAD_H
#define TRINAING_THREAD_H

#include <QCoreApplication>
#include <QString>
#include <QThread>
#include <QDebug>
#include <QVector>
#include "training_thread_item.h"
#include "training_thread_message.h"
#include "../interfaces/iwebsocketserver.h"

class TrainingThread : public QThread{
		Q_OBJECT
		IWebSocketServer *m_pWebSocketServer;
		int m_nSleep;
		
		void sendMessage(TrainingThreadMessage &msg);
		QVector<TrainingThreadItem *> sortedList();
	private:
		TrainingThreadMessage m_lastMessage;
	public:
		TrainingThread(IWebSocketServer *pWebSocketServer);
	protected:
		void run();
};

#endif // TRINAING_THREAD_H
