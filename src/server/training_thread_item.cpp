#include "training_thread_item.h"

#include <vertex_graph.h>
#include <helpers.h>
#include <memory.h>
#include <memoryitem.h>
#include <QFile>

TrainingThreadItem::TrainingThreadItem(int bitid){
	m_nBitid = bitid;
	m_sBitid = "bit" + QString::number(bitid).rightJustified(3, '0');
	m_sFilename = "/usr/share/reversehashd/md5/" + m_sBitid + ".vertexgraph";
	VertexGraph vg(128);
	vg.loadFromFile(m_sFilename);
	m_nPercent = vg.lastSuccessPersents();
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

int TrainingThreadItem::percent(){
	return m_nPercent;
}

int TrainingThreadItem::percent() const{
	return m_nPercent;
}
