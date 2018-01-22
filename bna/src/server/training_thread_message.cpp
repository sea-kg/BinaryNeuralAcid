#include "training_thread_message.h"

#include <memory.h>
#include <QFile>

TrainingThreadMessage::TrainingThreadMessage(int nBitId){
    m_nBitid = nBitId;
    m_sBitid = QString::number(m_nBitid).rightJustified(3, '0');
    m_nLastSuccessPersents = -1;
    m_nPercent = -1;
    m_nCompletedExperiments = -1;
    m_nMaxExperiments = -1;
}

int TrainingThreadMessage::id(){
	return m_nBitid;
}

QString TrainingThreadMessage::bitid(){
	return m_sBitid;
}

void TrainingThreadMessage::setMessage(QString message){
	m_sMessage = message;
}

QString TrainingThreadMessage::message(){
	return m_sMessage;
}

bool TrainingThreadMessage::equals(TrainingThreadMessage *pMsg){
    return pMsg->message() == m_sMessage
        && m_nLastSuccessPersents == pMsg->lastSuccessPersents()
        && m_nPercent == pMsg->percent()
        && m_nBitid == pMsg->id()
        && m_sBitid == pMsg->bitid()
        && m_nCompletedExperiments == pMsg->completedExperiments()
        && m_nMaxExperiments == pMsg->maxExperiments();
}

void TrainingThreadMessage::setLastSuccessPersents(int lp){
	m_nLastSuccessPersents = lp;
}

int TrainingThreadMessage::lastSuccessPersents(){
	return m_nLastSuccessPersents;
}

void TrainingThreadMessage::setPercent(int nPersent){
	m_nPercent = nPersent;
}

int TrainingThreadMessage::percent(){
	return m_nPercent;
}

void TrainingThreadMessage::setMaxExperiments(int nMaxExperiments){
	m_nMaxExperiments = nMaxExperiments;
}

int TrainingThreadMessage::maxExperiments(){
	return m_nMaxExperiments;
}

void TrainingThreadMessage::setCompletedExperiments(int nCompletedExperiments){
	m_nCompletedExperiments = nCompletedExperiments;
}

int TrainingThreadMessage::completedExperiments(){
	return m_nCompletedExperiments;
}

QJsonObject TrainingThreadMessage::toJson(){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue("training_thread_info");
	jsonData["rid"] = QJsonValue(0);
	jsonData["bitid"] = m_sBitid;
	jsonData["result"] = "OK";
	jsonData["status"] = m_sMessage;
	if(m_nLastSuccessPersents >= 0){
		jsonData["lp"] = m_nLastSuccessPersents;
	}
	
	if(m_nPercent >= 0){
		jsonData["percent"] = m_nPercent;
	}
	
	if(m_nCompletedExperiments >= 0){
		jsonData["completed"] = m_nCompletedExperiments;
	}
	
	if(m_nMaxExperiments >= 0){
		jsonData["max"] = m_nMaxExperiments;
	}

	return jsonData;
}
