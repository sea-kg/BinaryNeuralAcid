#ifndef TRINAING_THREAD_ITEM_H
#define TRINAING_THREAD_ITEM_H

#include <QString>

class TrainingThreadItem {
	public:
		TrainingThreadItem(int bitid);
		int id();
		int id() const;
		QString bitid();
		QString filename();
		int percent() const;
		void savePercent(int new_persent);
	private:
		QString m_sFilename;
		QString m_sFilenameStats;
		int m_nPercent;
		QString m_sBitid;
		int m_nBitid;
};

#endif // TRINAING_THREAD_ITEM_H
