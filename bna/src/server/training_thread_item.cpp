#include "training_thread_item.h"

#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <helpers.h>

TrainingThreadItem::TrainingThreadItem(int bitid){
	m_nBitid = bitid;
	QString name = QString::number(bitid).rightJustified(3, '0');
	QString subdir = name[0] + "/" + name[1] + "/" + name[2];
	m_sBitid = name;
    m_sFilename = "tests_bna_md5/" + subdir + "/" + name + ".bna";
    m_sFilenameStats = "tests_bna_md5/" + subdir + "/" + name + ".statistics";
	m_nPercent = 0;
	
	m_nPercent = loadPersent(m_sFilenameStats);
}

int TrainingThreadItem::id(){
	return m_nBitid;
}

int TrainingThreadItem::id() const{
	return m_nBitid;
}

QString TrainingThreadItem::bitid(){
	return m_sBitid;
}

QString TrainingThreadItem::filename(){
	return m_sFilename;
}

int TrainingThreadItem::percent() const{
	return m_nPercent;
}

void TrainingThreadItem::savePercent(int v){
	m_nPercent = v;
	savePersent(m_sFilenameStats, m_nPercent);
}
