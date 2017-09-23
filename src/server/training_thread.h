#ifndef TRINAING_THREAD_H
#define TRINAING_THREAD_H

#include <QCoreApplication>
#include <QString>
#include <QThread>
#include <QDebug>
#include <QVector>
#include "training_thread_item.h"
#include "training_thread_message.h"
#include <ireversehashdserver.h>

class TrainingThread : public QThread{
		Q_OBJECT
		IReverseHashDServer *m_pReverseHashDServer;
		int m_nSleep;
		
		void sendMessage(TrainingThreadMessage &msg);
		QVector<TrainingThreadItem *> sortedList();
	private:
		TrainingThreadMessage m_lastMessage;
	public:
		TrainingThread(IReverseHashDServer *pReverseHashDServer);
	protected:
		void run();
};

#endif // TRINAING_THREAD_H
