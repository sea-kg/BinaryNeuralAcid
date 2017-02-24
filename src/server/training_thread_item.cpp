#include "training_thread_item.h"

#include "../vertex_graph.h"
#include "../helpers.h"
#include "../memory.h"
#include "../memoryitem.h"
#include <QFile>

TrainingThreadItem::TrainingThreadItem(int bitid){
	m_sBitid = "bit" + QString::number(bitid).rightJustified(3, '0');
	m_sFilename = "/usr/share/reversehashd/md5/" + m_sBitid + ".vertexgraph";
	reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
	pVertexGraph->loadFromFile(m_sFilename);
	m_nPercent = pVertexGraph->lastSuccessPersents();
	delete pVertexGraph;
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
