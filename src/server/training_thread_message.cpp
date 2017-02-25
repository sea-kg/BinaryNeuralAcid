#include "training_thread_message.h"

#include "../vertex_graph.h"
#include "../helpers.h"
#include "../memory.h"
#include "../memoryitem.h"
#include <QFile>

TrainingThreadMessage::TrainingThreadMessage(){
	m_nBitid = -1;
	m_sBitid = "nope";
}

TrainingThreadMessage::TrainingThreadMessage(TrainingThreadItem *pItem){
	m_nBitid = pItem->id();
	m_sBitid = pItem->bitid();
	/*m_sFilename = "/usr/share/reversehashd/md5/" + m_sBitid + ".vertexgraph";
	VertexGraph vg(128);
	vg.loadFromFile(m_sFilename);
	m_nPercent = vg.lastSuccessPersents();*/
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

bool TrainingThreadMessage::equals(TrainingThreadMessage &msg){
	return (msg.message() == m_sMessage);
}

int TrainingThreadMessage::percent(){
	return m_nPercent;
}

int TrainingThreadMessage::percent() const{
	return m_nPercent;
}
