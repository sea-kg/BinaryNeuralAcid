#ifndef TRINAING_THREAD_MESSAGE_H
#define TRINAING_THREAD_MESSAGE_H

#include <QCoreApplication>
#include <QString>
#include <QThread>
#include <QDebug>
#include <QStringList>
#include "training_thread_item.h"

class TrainingThreadMessage {
	public:
		TrainingThreadMessage();
		TrainingThreadMessage(TrainingThreadItem *pItem);
		int id();
		QString bitid();
		void setMessage(QString message);
		QString message();
		bool equals(TrainingThreadMessage &msg);
		
		int percent();
		int percent() const;
	private:
		QString m_sMessage;
		int m_nPercent;
		QString m_sBitid;
		int m_nBitid;
		
};

#endif // TRINAING_THREAD_MESSAGE_H
