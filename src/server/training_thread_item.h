#ifndef TRINAING_THREAD_ITEM_H
#define TRINAING_THREAD_ITEM_H

#include <QCoreApplication>
#include <QString>
#include <QThread>
#include <QDebug>
#include <QStringList>

class TrainingThreadItem {
	public:
		TrainingThreadItem(int bitid);
		QString bitid();
		QString filename();
		int percent();
		int percent() const;
	private:
		QString m_sFilename;
		int m_nPercent;
		QString m_sBitid;
		
};

#endif // TRINAING_THREAD_ITEM_H
