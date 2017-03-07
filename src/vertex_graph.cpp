#include "vertex_graph.h"
#include "vertexes/vertex_in.h"
#include "vertexes/vertex.h"
#include <iostream>
#include <cstring>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QTextStream>
#include <QBuffer>
#include <QDebug>
#include <QJsonObject>

VertexGraph::VertexGraph(int nInputs){
	m_pOut = NULL;
	m_nInputs = nInputs;
	m_nVersion = 1;
	m_nLastSuccessPersents = 0;
};

// -----------------------------------------------------------------

VertexGraph::~VertexGraph(){
	m_vVertexIn.clear();
	m_pOut = NULL;
	for(int i = m_vVertexes.size()-1; i >= 0; i--){
		IVertexOut *pVertex = m_vVertexes[i];
		m_vVertexes.remove(i);
		delete pVertex;
	}
}

// -----------------------------------------------------------------

void VertexGraph::setLastSuccessPersents(int nVal){
	m_nLastSuccessPersents = nVal;
};

// -----------------------------------------------------------------

int VertexGraph::lastSuccessPersents(){
	return m_nLastSuccessPersents;
};
		
// -----------------------------------------------------------------

void VertexGraph::genBase(){
	m_vVertexIn.clear();
	m_pOut = NULL;
	m_vVertexes.clear();
	m_nLastSuccessPersents = 0;

	QVector<IVertexOut *> vTmp;
	for(int i = 0; i < m_nInputs; i++){
		IVertexIn *pVertexIn = new VertexIn();
		pVertexIn->setValue(false);
		pVertexIn->setNumber(i);
		IVertexOut *pVertexOut = dynamic_cast<IVertexOut *>(pVertexIn);
		pVertexOut->setName("i" + QString::number(i));
		m_vVertexIn.push_back(pVertexIn);
		m_vVertexes.push_back(pVertexOut);
		vTmp.push_back(pVertexOut);
	}
	int mI = 0;
	while(vTmp.size() > 2){
		QVector<IVertexOut *> vTmp2;
		for(int i = 0; i < vTmp.size()/2; i++){
			IVertexOut *pVertexOut = new Vertex();
			pVertexOut->setName("m" + QString::number(mI));
			mI++;
			IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
			pVertexOperation->setOperation("|");
			pVertexOperation->setIn1(vTmp[i*2]);
			pVertexOperation->setIn2(vTmp[i*2+1]);
			vTmp2.push_back(pVertexOut);
			m_vVertexes.push_back(pVertexOut);
		}
		vTmp.clear();
		for(int i = 0; i < vTmp2.size(); i++){
			vTmp.push_back(vTmp2[i]);
		}
		vTmp2.clear();
	}
	m_pOut = new Vertex();
	m_pOut->setName("out");
	m_vVertexes.push_back(m_pOut);
	IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(m_pOut);
	pVertexOperation->setIn1(vTmp[0]);
	pVertexOperation->setIn2(vTmp[1]);
	pVertexOperation->setOperation("|");
};

// -----------------------------------------------------------------

bool VertexGraph::out(){
	if(this->findCicles()){
		qDebug().noquote().nospace() << " !!! Error: Found cicles";
		return false;
	}
	return m_pOut->out();
}

// -----------------------------------------------------------------

IVertexOut *VertexGraph::vertexOut(){
	return m_pOut;
}

// -----------------------------------------------------------------

void VertexGraph::setIn(const QVector<bool> &in){
	int nVertexInSize = m_vVertexIn.size();
	int nInSize = in.size();
	if(nInSize > nVertexInSize){
		qDebug().noquote().nospace() << "VERTEX_GRAPH: Warning input to so much. (" << m_sFilename << ")";
		qDebug().noquote().nospace() << nInSize << " != " << nVertexInSize << "\n";
	}
	for(int i = 0; i < nInSize; i++){
		if(i < nVertexInSize){
			m_vVertexIn[i]->setValue(in[i]);
		}
	}
	if(nInSize < nVertexInSize){
		for(int i = nInSize; i < nVertexInSize; i++){
			m_vVertexIn[i]->setValue(false);
		}
	}
}

// -----------------------------------------------------------------

void VertexGraph::recalculateCoordinates(){
	for(int i = 0; i < m_vVertexes.size(); i++){
		float x = i%25;
		float y = (i - i%25)/25;
		float z = 0;
		m_vVertexes[i]->setXYZ(x, y, z);
	}
	
	int perturbation = 1;
	while(perturbation > 0){
		perturbation = 0;
		for(int i = 0; i < m_vVertexes.size(); i++){
			IVertexOut *pVertex = m_vVertexes[i];
			if(pVertex->type() != "VertexIn"){
				IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertex);
				float y = pVertex->y() + 1;
				if(pVertexOperation->in1()->y() != y){
					pVertexOperation->in1()->setY(y);
					perturbation++;
				}
				
				if(pVertexOperation->in2()->y() != y){
					pVertexOperation->in2()->setY(y);
					perturbation++;
				}
			}
		}
	}
	
	/*QMap<int,int> levels;
	for(int i = 0; i < m_vVertexes.size(); i++){
		int y = int(m_vVertexes[i]->y());
		if(!levels.contains(y)){
			levels[y] = 1;
		}else{
			levels[y]++;
		}
	}

	int max = 0;
	foreach(int key, levels.keys()){
		int count = levels.value(key);
		if(count > max){
			max = count;
		}
	}*/

	perturbation = 1;
	while(perturbation > 0){
		perturbation = 0;
		QMap<int,int> levels;
		for(int i = 0; i < m_vVertexes.size(); i++){
			IVertexOut *pVertex = m_vVertexes[i];
			int y = pVertex->y();
			if(!levels.contains(y)){
				levels[y] = 0;
			}else{
				levels[y] = levels[y] + 1;
			}
			int x = levels[y];
			if(pVertex->x() != x){
				pVertex->setX(x);
				perturbation++;
			}
		}
	}
}

// -----------------------------------------------------------------

QJsonArray VertexGraph::conv2json(){
	QJsonArray result;
	
	for(int i = 0; i < m_vVertexes.size(); i++){
		IVertexOut *pVertex = m_vVertexes[i];
		
		QJsonObject vertex;
		QString vertexName = pVertex->name();
		vertex["id"] = vertexName;
		if(pVertex->type() == "VertexIn"){
			vertex["type"] = "in";
		}else{
			vertex["type"] = vertexName == "out" ? "out" : "m";
			IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertex);
			vertex["in1"] = pVertexOperation->in1()->name();
			vertex["in2"] = pVertexOperation->in2()->name();
			vertex["o"] = pVertexOperation->operation();
		}
		result.append(vertex);
	}
	return result;
}

// -----------------------------------------------------------------

void addLine(QString &result, IVertexOut *pTo, IVertexOut *pFrom){
	
	if(pTo->type() == "Vertex"){
		result += "\t" + pFrom->name() + " ->" + pTo->name() + ";\n";
		if(pFrom->type() == "VertexIn"){
			result += "\t" + pFrom->name() + " [ label = \"" + pFrom->name() + "\" ];\n";	
		}else{
			IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pFrom);
			result += "\t" + pFrom->name() + " [ label = \"" + pVertexOperation->operation() + "\" ];\n";
			addLine(result, pFrom, pVertexOperation->in1());
			addLine(result, pFrom, pVertexOperation->in2());
		}
	}
}

// -----------------------------------------------------------------

QString VertexGraph::conv2dot(){
	QString result = "";
	result += "digraph reversehash {\n";
	result += "\t" + m_pOut->name() + " [ label = \"out\" ];\n";
	
	IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(m_pOut);
	addLine(result, m_pOut, pVertexOperation->in1());
	addLine(result, m_pOut, pVertexOperation->in2());

	result += "}\n";
	return result;
}

// -----------------------------------------------------------------

void VertexGraph::writeHeader(QDataStream &stream, int nVersion){
	stream.writeRawData("VERTEXGRAPH", 11);
	stream << nVersion; // version
	stream << m_nLastSuccessPersents; // lastSuccessPersents
}

// -----------------------------------------------------------------

bool VertexGraph::readHeader(QDataStream &stream, int &nVersion){
	char *pFileType = new char[12];
	std::memset(pFileType, 0, 12);
	int nReaded = stream.readRawData(pFileType, 11);
	if(nReaded > 0){
		if(QString(pFileType) != "VERTEXGRAPH"){
			qDebug().noquote().nospace() << "VERTEXGRAPH: File type did not match with VERTEXGRAPH.\n";
			return false;
		}
	}else{
		qDebug().noquote().nospace() << "VERTEXGRAPH: Could not read file (1)\n";
		return false;
	}
	stream >> nVersion;
	stream >> m_nLastSuccessPersents;
	return true;
}

// -----------------------------------------------------------------

void VertexGraph::writeDataAsVersion1(QDataStream &stream){
	stream << m_vVertexes.size();
	for (int i = 0; i < m_vVertexes.size(); i++) {
		reversehash::IVertexOut *pVertexOut = m_vVertexes[i];
		stream << pVertexOut->type().toUtf8();
		stream << pVertexOut->name().toUtf8();
		stream << pVertexOut->x();
		stream << pVertexOut->y();
		stream << pVertexOut->z();
		
		if(pVertexOut->type() == "Vertex"){
			IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
			stream << pVertexOperation->operation().toUtf8();
			if(pVertexOperation->in1() == NULL){
				qDebug().noquote().nospace() << "VERTEX_GRAPH (writeDataAsVersion1): Failed in1 is NULL for " << pVertexOut->name();
			}
			
			stream << pVertexOperation->in1()->type().toUtf8();
			stream << pVertexOperation->in1()->name().toUtf8();
			
			if(pVertexOperation->in2() == NULL){
				qDebug().noquote().nospace() << "VERTEX_GRAPH (writeDataAsVersion1): Failed in2 is NULL for " << pVertexOut->name();
			}
			
			stream << pVertexOperation->in2()->type().toUtf8();
			stream << pVertexOperation->in2()->name().toUtf8();
		}else if(pVertexOut->type() == "VertexIn"){
			IVertexIn *pVertexIn = dynamic_cast<IVertexIn *>(pVertexOut);
			stream << pVertexIn->number();
		}
	}
}

// -----------------------------------------------------------------

IVertexOut *VertexGraph::findVertexByName(QString name){
	IVertexOut *pResult = NULL;
	for(int i = 0; i < m_vVertexes.size(); i++){
		if(m_vVertexes[i]->name() == name){
			pResult = m_vVertexes[i];
		}
	}
	return pResult;
}

// -----------------------------------------------------------------

void VertexGraph::readDataAsVersion1(QDataStream &stream){
	int nSize = 0;
	stream >> nSize;
	for(int i = 0; i < nSize; i++){
		QByteArray type;
		stream >> type;
		QByteArray name;
		stream >> name;
		float x,y,z;
		stream >> x;
		stream >> y;
		stream >> z;

		if(type == "Vertex"){
			IVertexOut *pVertexOut = this->findVertexByName(QString(name));
			if(pVertexOut == NULL){
				pVertexOut = new Vertex();
				m_vVertexes.push_back(pVertexOut);
			}
			pVertexOut->setName(QString(name));
			pVertexOut->setXYZ(x,y,z);
			IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
			QByteArray operation;
			stream >> operation;
			pVertexOperation->setOperation(QString(operation));

			QByteArray in1type;
			QByteArray in1name;
			QByteArray in2type;
			QByteArray in2name;

			stream >> in1type;
			stream >> in1name;
			stream >> in2type;
			stream >> in2name;

			IVertexOut *pVertexIn1 = this->findVertexByName(QString(in1name));
			IVertexOut *pVertexIn2 = this->findVertexByName(QString(in2name));

			if(pVertexIn1 == NULL){
				if(QString(in1type) == "Vertex"){
					pVertexIn1 = new Vertex();
				}else if(QString(in1type) == "VertexIn"){
					pVertexIn1 = new VertexIn();
				}
				pVertexIn1->setName(QString(in1name));
				m_vVertexes.push_back(pVertexIn1);
			}

			if(pVertexIn2 == NULL){
				if(QString(in2type) == "Vertex"){
					pVertexIn2 = new Vertex();
				}else if(QString(in2type) == "VertexIn"){
					pVertexIn2 = new VertexIn();
				}
				pVertexIn2->setName(QString(in2name));
				m_vVertexes.push_back(pVertexIn2);
			}

			pVertexOperation->setIn1(pVertexIn1);
			pVertexOperation->setIn2(pVertexIn2);

			if(QString(name) == "out"){
				m_pOut = pVertexOut;
			}
		}else if(type == "VertexIn"){
			IVertexIn *pVertexIn = new VertexIn();
			int nNumber = -1;
			stream >> nNumber;
			m_vVertexIn.push_back(pVertexIn);
			IVertexOut *pVertexOut = dynamic_cast<IVertexOut *>(pVertexIn);
			pVertexOut->setName(QString(name));
			pVertexOut->setXYZ(x,y,z);
			m_vVertexes.push_back(pVertexOut);
		}
	}
}

// -----------------------------------------------------------------

bool VertexGraph::saveToFile(QString filename){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug().noquote().nospace() << "VERTEX_GRAPH: Could not write file: " << filename;
		return false;
	}
	QDataStream stream( &file );
	this->saveToStream(stream);
	file.close();
	return true;
}

// -----------------------------------------------------------------

bool VertexGraph::saveToStream(QDataStream &stream){
	this->writeHeader(stream, m_nVersion);
	this->writeDataAsVersion1(stream);
	return true;
}

// -----------------------------------------------------------------

bool VertexGraph::saveDot(QString filename){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug().noquote().nospace() << "VERTEX_GRAPH: Could not write file: " << filename;
		return false;
	}
	QTextStream stream( &file );
	stream << conv2dot();
	file.close();
	return true;
}

// -----------------------------------------------------------------

bool VertexGraph::loadFromFile(QString filename){
	QFile file(filename);
	if (!file.exists()) {
		qDebug().noquote().nospace() << "VERTEX_GRAPH: File did not exists: " << filename;
		return false;
	}
	if ( !file.open(QIODevice::ReadOnly) ) {
		qDebug().noquote().nospace() << "VERTEX_GRAPH: Could not open file " << filename;
		return false;
	}

	m_sFilename = filename;
	QDataStream stream( &file );
	
	return this->loadFromStream(stream);
}


// -----------------------------------------------------------------

bool VertexGraph::loadFromStream(QDataStream &stream){
	m_vVertexIn.clear();
	m_pOut = NULL;
	for(int i = m_vVertexes.size()-1; i >= 0; i--){
		IVertexOut *pVertex = m_vVertexes[i];
		m_vVertexes.remove(i);
		delete pVertex;
	}
	m_vVertexes.clear();

	int nVersion = -1;
	if(!this->readHeader(stream, nVersion)){
		return false;
	}

	if(nVersion == 1){
		this->readDataAsVersion1(stream);
	}else{
		return false;
	}

	return true;
}

// -----------------------------------------------------------------

VertexGraph * VertexGraph::clone(){
	QByteArray arr;
	QBuffer buffer(&arr);
	buffer.open(QIODevice::WriteOnly);
	QDataStream stream( &buffer );
	this->saveToStream(stream);

	QBuffer buffer2(&arr);
	buffer2.open(QIODevice::ReadOnly);
	QDataStream stream2( &buffer2 );
	VertexGraph *pVertexGraph = new VertexGraph(m_nInputs);
	pVertexGraph->loadFromStream(stream2);
	buffer.close();
	buffer2.close();
	arr.clear();
	return pVertexGraph;
}

// -----------------------------------------------------------------

void VertexGraph::copy(VertexGraph *pVertexGraph){
	QByteArray arr;
	QBuffer buffer(&arr);
	buffer.open(QIODevice::WriteOnly);
	QDataStream stream( &buffer );
	pVertexGraph->saveToStream(stream);
	
	QBuffer buffer2(&arr);
	buffer2.open(QIODevice::ReadOnly);
	QDataStream stream2( &buffer2 );
	this->loadFromStream(stream2);
	buffer.close();
	buffer2.close();
	arr.clear();
}

// -----------------------------------------------------------------

void VertexGraph::printStackVertexes(QVector<reversehash::IVertexOut *> &stack){
	for(int i = 0; i < stack.size(); i++){
		if(stack[i] != NULL){
			qDebug().noquote().nospace() << "VERTEX_GRAPH: Stack " << i << ":" << stack[i]->name();
		}
	}
}

// -----------------------------------------------------------------

bool VertexGraph::findCiclesRecourse(reversehash::IVertexOut *pVertexOut, QVector<reversehash::IVertexOut *> &stack){
	if(pVertexOut == NULL){
		qDebug().noquote().nospace() << "VERTEX_GRAPH: Error(findCiclesRecourse): Some vertext is NULL";
		return true;
	}
	if(pVertexOut->type() == "Vertex"){
		IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
		if(pVertexOperation->in1() == NULL){
			qDebug().noquote().nospace() << "VERTEX_GRAPH: Error(findCiclesRecourse): Some vertext is NULL (1) for " << pVertexOut->name();
			printStackVertexes(stack);
		}
		if(pVertexOperation->in2() == NULL){
			qDebug().noquote().nospace() << "VERTEX_GRAPH: Error(findCiclesRecourse): Some vertext is NULL (2) for " << pVertexOut->name();
		}
		
		for(int i = 0; i < stack.size(); i++){
			if(pVertexOperation->in1() == stack[i] || pVertexOperation->in2() == stack[i]){
				return true;
			}
		}
		
		stack.push_back(pVertexOperation->in1());
		if(findCiclesRecourse(pVertexOperation->in1(), stack))
			return true;
		stack.pop_back();
		
		
		stack.push_back(pVertexOperation->in2());
		if(findCiclesRecourse(pVertexOperation->in2(), stack))
			return true;
		stack.pop_back();
	}
	return false;
}

// -----------------------------------------------------------------

bool VertexGraph::findCicles(){
	QVector<IVertexOut *> stack;
	stack.push_back(m_pOut);
	bool bResult = findCiclesRecourse(m_pOut, stack);
	stack.pop_back();
	return bResult;
}

// -----------------------------------------------------------------

int VertexGraph::countOfVertextes(){
	return m_vVertexes.size();
}

// -----------------------------------------------------------------

void VertexGraph::changeRandomOperation(){
	bool bChanged = false;
	while(!bChanged){
		QString opers = "|&^";
		int n = qrand() % (m_vVertexes.size());
		IVertexOut *pVertexOut = m_vVertexes[n];
		if(pVertexOut->name() != "out" && pVertexOut->type() != "VertexIn"){
			IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
			QString newOper = QString(opers[qrand() % opers.length()]);
			pVertexOperation->setOperation(newOper);
			bChanged = true;
		}else{
			bChanged = false;
		}
	}
}

// -----------------------------------------------------------------

IVertexOut *VertexGraph::findRandomVertex(){
	bool bFound = false;
	int tries = 0;
	IVertexOut *pVertexOut = NULL;
	while(!bFound){
		int nVertex = qrand() % (m_vVertexes.size());
		pVertexOut = m_vVertexes[nVertex];
		if(pVertexOut->type() == "Vertex"){
			bFound = true;
		}
		tries++;
		if(tries > 100){
			return NULL;
		}
	}
	return pVertexOut;
}

// -----------------------------------------------------------------

bool VertexGraph::findIntersectionRecourse(IVertexOut *pVertexStart, IVertexOut *pVertexSearch){
	if(pVertexStart == pVertexSearch){
		return true;
	}
	
	if(pVertexStart == NULL){
		qDebug().noquote().nospace() << "VERTEX_GRAPH: !!!findIntersectionRecourse Error: Some vertext is NULL";
		return true;
	}
	if(pVertexStart->type() == "Vertex"){
		IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexStart);
		if(pVertexOperation->in1() == pVertexSearch || pVertexOperation->in2() == pVertexSearch){
			return true;
		}
		if(pVertexOperation->in1() == NULL){
			qDebug().noquote().nospace() << "VERTEX_GRAPH: !!!findIntersectionRecourse Error: Some vertext is NULL (1) for " << pVertexStart->name();
		}
		if(findIntersectionRecourse(pVertexOperation->in1(), pVertexSearch))
			return true;

		if(pVertexOperation->in2() == NULL){
			qDebug().noquote().nospace() << "VERTEX_GRAPH: !!!findIntersectionRecourse Error: Some vertext is NULL (2)" << pVertexStart->name();
		}
		if(findIntersectionRecourse(pVertexOperation->in2(), pVertexSearch))
			return true;
	}
	return false;
}

// -----------------------------------------------------------------

void VertexGraph::swapRandomVertextIns(){
	
	IVertexOut *pVertexOut1 = findRandomVertex();
	IVertexOut *pVertexOut2 = findRandomVertex();

	if(pVertexOut1 == NULL || pVertexOut2 == NULL) return;
	
	IVertexOperation *pVertexOperation1 = dynamic_cast<IVertexOperation *>(pVertexOut1);
	IVertexOperation *pVertexOperation2 = dynamic_cast<IVertexOperation *>(pVertexOut2);

	IVertexOut *pOut1 = pVertexOperation1->in1();
	IVertexOut *pOut2 = pVertexOperation2->in1();

	if(pOut1 == NULL || pOut2 == NULL) {
		qDebug().noquote().nospace() << "VERTEX_GRAPH: Some pOut1 or pOut2 is NULL (1)";
		return;
	}
	
	if(pOut1 == pVertexOut1 || pVertexOut1 == pVertexOut2 || pOut2 == pVertexOut2){
		return;
	}

	int nTries = 0;
	bool bIntersec1 = findIntersectionRecourse(pOut1, pVertexOut2);
	bool bIntersec2 = findIntersectionRecourse(pOut2, pVertexOut1);
	while(bIntersec1 || bIntersec2 || pVertexOut1 == pVertexOut2) {
		pVertexOut1 = findRandomVertex();
		pVertexOut2 = findRandomVertex();

		if(pVertexOut1 == NULL || pVertexOut2 == NULL) return;

		pVertexOperation1 = dynamic_cast<IVertexOperation *>(pVertexOut1);
		pVertexOperation2 = dynamic_cast<IVertexOperation *>(pVertexOut2);

		pOut1 = pVertexOperation1->in1();
		pOut2 = pVertexOperation2->in1();
		
		if(pOut1 == NULL || pOut2 == NULL) {
			qDebug().noquote().nospace() << "Some pOut1 or pOut2 is NULL (2)\n";
			return;
		}
		
		if(pOut1 == pVertexOut1 || pVertexOut1 == pVertexOut2 || pOut2 == pVertexOut2){
			return;
		}
		
		nTries++;
		if(nTries > 100){
			return;
		}
	};
	
	pVertexOperation1->setIn1(pOut2);
	pVertexOperation2->setIn1(pOut1);
}

// -----------------------------------------------------------------

void VertexGraph::randomRemoveVertex(){
	IVertexOperation *pVertexOperationRemoved = NULL;
	IVertexOut *pVertexOutRemoved = NULL;

	bool bFound = false;
	int nVertex = -1;
	int tries = 0;
	while(!bFound){
		nVertex = qrand() % (m_vVertexes.size());
		pVertexOutRemoved = m_vVertexes[nVertex];
		if(pVertexOutRemoved->name() != "out" && pVertexOutRemoved->type() != "VertexIn"){
			pVertexOperationRemoved = dynamic_cast<IVertexOperation *>(pVertexOutRemoved);
			bFound = true;
		}
		tries++;
		if(tries > 100){
			return;
		}
	}
	
	IVertexOut *pVertexOut1 = pVertexOperationRemoved->in1();
	IVertexOut *pVertexOut2 = pVertexOperationRemoved->in2();

	for(int i = 0; i < m_vVertexes.size(); i++){
		IVertexOut *pVertexOut = m_vVertexes[i];
		if(pVertexOut->type() == "Vertex"){
			IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
			
			if(pVertexOperation->in1() == pVertexOutRemoved){
				int n = qrand() % (2);
				if(n == 0){
					pVertexOperation->setIn1(pVertexOut1);
				}else{
					pVertexOperation->setIn1(pVertexOut2);
				}
			}
			if(pVertexOperation->in2() == pVertexOutRemoved){
				int n = qrand() % (2);
				if(n == 0){
					pVertexOperation->setIn2(pVertexOut1);
				}else{
					pVertexOperation->setIn2(pVertexOut2);
				}
			}
		}
	}
	m_vVertexes.remove(nVertex);
}

// -----------------------------------------------------------------

void VertexGraph::randomAddVertex(){
	if(m_vVertexes.size() > 500){
		return;
	}

	IVertexOperation *pVertexOperationTop = NULL;
	IVertexOut *pVertexOutTop = NULL;

	bool bFound = false;
	int nVertex = -1;
	int tries = 0;
	while(!bFound){
		nVertex = qrand() % (m_vVertexes.size());
		pVertexOutTop = m_vVertexes[nVertex];
		if(pVertexOutTop->name() != "out" && pVertexOutTop->type() != "VertexIn"){
			pVertexOperationTop = dynamic_cast<IVertexOperation *>(pVertexOutTop);
			bFound = true;
		}
		tries++;
		if(tries > 100){
			return;
		}
	}
	
	IVertexOut *pVertexOut1 = pVertexOperationTop->in1();
	IVertexOut *pVertexOut2 = pVertexOperationTop->in2();

	IVertexOut *pVertexOutAdd = new Vertex();
	IVertexOperation *pVertexOperationAdd = dynamic_cast<IVertexOperation *>(pVertexOutAdd);
	
	int nOper = qrand() % (3);
	if(nOper == 0){
		pVertexOperationAdd->setOperation("|");
	}else if(nOper == 1){
		pVertexOperationAdd->setOperation("&");
	}else{
		pVertexOperationAdd->setOperation("^");
	}

	// generate vertex name
	int mI = 1;
	QString vertexName = "m" + QString::number(mI);
	IVertexOut *pVertexTest = this->findVertexByName(vertexName);
	while(pVertexTest != NULL){
		mI++;
		vertexName = "m" + QString::number(mI);
		pVertexTest = this->findVertexByName(vertexName);
	}
	pVertexTest = NULL;
	pVertexOutAdd->setName(vertexName);

	m_vVertexes.insert(nVertex, pVertexOutAdd);

	// set ins
	int nIns = qrand() % (2);
	if(nIns == 0){
		pVertexOperationAdd->setIn1(pVertexOut1);
		pVertexOperationAdd->setIn2(pVertexOut2);
	}else{
		pVertexOperationAdd->setIn1(pVertexOut2);
		pVertexOperationAdd->setIn2(pVertexOut1);
	}

	// broke vertexes connection
	int nIn = qrand() % (2);
	if(nIn == 0){
		pVertexOperationTop->setIn1(pVertexOutAdd);
	}else{
		pVertexOperationTop->setIn2(pVertexOutAdd);
	}
}

// -----------------------------------------------------------------

void VertexGraph::randomChanges(int count){
	int nPossibleRandomChanges = 4;
	// replace operations
	for(int i = 0; i < count; i++){
		int n = qrand() % (nPossibleRandomChanges);
		bool bFound = false;
		int tries = 0;
		while(!bFound){
			qDebug().noquote().nospace() << "VERTEX_GRAPH: Clone graph " << i;
			VertexGraph *pVertexGraphClone = this->clone();
			switch(n){
				case 0:
					qDebug().noquote().nospace() << "VERTEX_GRAPH: Random operation " << i;
					pVertexGraphClone->changeRandomOperation();
					break;
				case 1:
					qDebug().noquote().nospace() << "VERTEX_GRAPH: Swap Random Vertext Ins " << i;
					pVertexGraphClone->swapRandomVertextIns();
					break;
				case 2:
					qDebug().noquote().nospace() << "VERTEX_GRAPH: Random Add Vertext " << i;
					pVertexGraphClone->randomAddVertex();
					break;
				case 3:
					qDebug().noquote().nospace() << "VERTEX_GRAPH: Random Remove Vertext " << i;
					pVertexGraphClone->randomRemoveVertex();
					break;
			}
			if(!pVertexGraphClone->findCicles()){
				qDebug().noquote().nospace() << "VERTEX_GRAPH: Copy graph " << i;
				this->copy(pVertexGraphClone);
				bFound = true;
			}
			delete pVertexGraphClone;
			tries++;
			if(tries > 100){
				return;
			}
		}
	}
}
