#ifndef TRAINING_THREAD_H
#define TRAINING_THREAD_H

// #include <QCoreApplication>
#include <QString>
#include <QThread>
#include <QVector>
#include "training_thread_message.h"
#include <ireversehashdserver.h>
#include <bna.h>

class TrainingThread : public QThread{
		Q_OBJECT
		IReverseHashDServer *m_pReverseHashDServer;
		int m_nSleep;
		
        void sendMessage(TrainingThreadMessage *pMsg);
	private:
        TrainingThreadMessage *m_pLastMessage;
        BNAProject *m_pBNAProject;
	public:
        TrainingThread(IReverseHashDServer *pReverseHashDServer, BNAProject *pBNAProject);
	protected:
		void run();
};

#endif // TRAINING_THREAD_H
