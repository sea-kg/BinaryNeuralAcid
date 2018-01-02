#include <bna.h>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

// -----------------------------------------------------------------
// BNA Var just contains boolean variable

BNAVar::BNAVar() {
   m_bVal = false;
   m_sName = "";
}

// -----------------------------------------------------------------

void BNAVar::name(QString name){
    m_sName = name;
}

// -----------------------------------------------------------------

QString BNAVar::name(){
    return m_sName;
}

// -----------------------------------------------------------------

bool BNAVar::val(){
    return m_bVal;
}

// -----------------------------------------------------------------

void BNAVar::val(bool bVal){
    m_bVal = bVal;
}

// -----------------------------------------------------------------
// BNA Expr class for calculation by operation

BNAExpr::BNAExpr(){
    m_pVar1 = NULL;
    m_pVar2 = NULL;
    m_pVarOut = NULL;
    m_pOper = NULL;
}

// -----------------------------------------------------------------

void BNAExpr::op1(BNAVar *pVar1){ m_pVar1 = pVar1; }
BNAVar *BNAExpr::op1(){ return m_pVar1; }
void BNAExpr::op2(BNAVar *pVar2){ m_pVar2 = pVar2; }
BNAVar * BNAExpr::op2(){ return m_pVar2; }
void BNAExpr::oper(IBNAOper *pOper){ m_pOper = pOper; }
IBNAOper * BNAExpr::oper(){ return m_pOper; }
void BNAExpr::out(BNAVar *pVarOut){ m_pVarOut = pVarOut; }
BNAVar * BNAExpr::out(){ return m_pVarOut; }

// -----------------------------------------------------------------

void BNAExpr::exec(){
    if(m_pVar1 == NULL){
        std::cerr << "[ERROR] m_pVar1 is NULL\n";
        return;
    }

    if(m_pVar2 == NULL){
        std::cerr << "[ERROR] m_pVar2 is NULL\n";
        return;
    }

    if(m_pVarOut == NULL){
        std::cerr << "[ERROR] m_pVarOut is NULL\n";
        return;
    }

    if(m_pOper == NULL){
        std::cerr << "[ERROR] m_pOper is NULL\n";
        return;
    }
    unsigned char b1 = m_pVar1->val() & 0x01;
    unsigned char b2 = m_pVar2->val() & 0x01;
    m_pVarOut->val(m_pOper->calc(b1, b2));
}

// -----------------------------------------------------------------
// Phisicly assotiation with expressions

BNAItem::BNAItem(unsigned short x, unsigned short y, unsigned char t){
    m_nX = x;
    m_nY = y;
    m_cT = t;
}

// -----------------------------------------------------------------

BNAItem::BNAItem() {
    m_nX = 0;
    m_nY = 0;
    m_cT = 0;
}

// -----------------------------------------------------------------

unsigned short BNAItem::getX(){
    return m_nX;
}

// -----------------------------------------------------------------

unsigned short BNAItem::getY(){
    return m_nY;
}

// -----------------------------------------------------------------

unsigned char BNAItem::getT(){
    return m_cT;
}

// -----------------------------------------------------------------

void BNAItem::setX(unsigned short x){
    m_nX = x;
}

// -----------------------------------------------------------------

void BNAItem::setY(unsigned short y){
    m_nY = y;
}

// -----------------------------------------------------------------

void BNAItem::setT(unsigned char t){
    m_cT = t;
}

// -----------------------------------------------------------------

void BNAItem::readXYT(QDataStream &stream){
    stream >> m_nX;
    stream >> m_nY;
    stream >> m_cT;
}

// -----------------------------------------------------------------

void BNAItem::writeXYT(QDataStream &stream){
    stream << m_nX << m_nY << m_cT;
}

// -----------------------------------------------------------------

QString BNAOperXor::type(){ return QString("XOR"); }
bool BNAOperXor::calc(bool b1, bool b2){ return b1 ^ b2; }

QString BNAOperNotXor::type(){ return QString("NXOR"); }
bool BNAOperNotXor::calc(bool b1, bool b2){ return !b1 ^ !b2; }

QString BNAOperAnd::type(){ return QString("AND"); }
bool BNAOperAnd::calc(bool b1, bool b2){ return b1 & b2; }

QString BNAOperOr::type(){ return QString("OR"); }
bool BNAOperOr::calc(bool b1, bool b2){ return b1 | b2; }

// -----------------------------------------------------------------

BNA::BNA(){
	m_nInput = 1; // Default
	m_nOutput = 1; // 16 bytes of md5 hash
    m_vOpers.push_back(new BNAOperXor());
    m_vOpers.push_back(new BNAOperNotXor());
    m_vOpers.push_back(new BNAOperAnd());
    m_vOpers.push_back(new BNAOperOr());
    m_nOperSize = m_vOpers.size();
}

// ----------------------------------------------------------------

BNA::~BNA(){
    m_vOpers.clear();
    m_vItems.clear();
    clearResources();
}

// ----------------------------------------------------------------

bool BNA::load(QString filename){
	QFile file(filename);
	if (!file.exists()) {
		qDebug().noquote().nospace() << "BNA:  File did not exists: " << filename;
		return false;
	}
	if (!file.open(QIODevice::ReadOnly) ) {
		qDebug().noquote().nospace() << "BNA: Could not open file " << filename;
		return false;
	}
	QDataStream stream(&file);
    m_vItems.clear();

	stream >> m_nInput;
	stream >> m_nOutput;

	while(!stream.atEnd()){
		BNAItem item;
        item.readXYT(stream);
		m_vItems.push_back(item);
	}
	normalize();
	return true;
}

// ----------------------------------------------------------------

bool BNA::save(QString filename){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename;
		return false;
	}
	QDataStream stream( &file );
	stream << m_nInput << m_nOutput;
	for (int i = 0; i < m_vItems.size(); i++) {
        m_vItems[i].writeXYT(stream);
	}
	file.close();
	return true;
};

// ----------------------------------------------------------------

void BNA::randomGenerate(int nInput, int nOutput, int nSize){
	m_nInput = nInput;
	m_nOutput = nOutput;
	nSize = nSize + m_nInput + m_nOutput;
	m_vItems.clear();
	for(int i = 0; i < nSize; i++){
		BNAItem item;
        item.setX(qrand());
        item.setY(qrand());
        item.setT(qrand());
		m_vItems.push_back(item);
	}
	normalize();
}

// ----------------------------------------------------------------

void BNA::normalize(){
    int nodes = m_nInput;
    for(int i = 0; i < m_vItems.size(); i++){
        m_vItems[i].setX(m_vItems[i].getX() % nodes);
        m_vItems[i].setY(m_vItems[i].getY() % nodes);
        m_vItems[i].setT(m_vItems[i].getT() % m_nOperSize);
        nodes++;
    }
    // prepare for calculations
    updateExprs();
}

// ----------------------------------------------------------------

bool BNA::exportToDot(QString filename, QString graphname){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename;
		return false;
	}
	QTextStream stream( &file );
	stream << "digraph " << graphname << " {\n";

    int nExprsSize = m_vCalcExprs.size();
    for(int i = 0; i < m_vCalcExprs.size(); i++){
        BNAExpr *pExpr = m_vCalcExprs[i];
        stream << "\t" << pExpr->out()->name() << " [label=\"" << pExpr->oper()->type() << "\"];\n";
        stream << "\t" << pExpr->op1()->name() << " -> " << pExpr->out()->name() << ";\n";
        stream << "\t" << pExpr->op2()->name() << " -> " << pExpr->out()->name() << ";\n";
        if(nExprsSize - i <= m_nOutput){
            stream << "\t" << pExpr->out()->name() << " -> out" << QString::number(nExprsSize - i) << ";\n";
        }
    }

    stream << "}\n";
	file.close();
	return true;
}

// ----------------------------------------------------------------

bool BNA::exportToCpp(QString filename, QString funcname){
	QFile file(filename);
	QFileInfo fi(filename);
	if(fi.suffix() != "cpp"){
		qDebug().noquote().nospace() << "[ERROR]" << filename << " file must be have suffix 'cpp'";
		return false;
	}
	
	QString filename_h = filename.left(filename.length() - 3);
	filename_h += "h";
	
	if (file.exists()) {
		file.remove();
	}
	
	QFile file_h(filename_h);
	if (file_h.exists()) {
		file_h.remove();
	}
	
	if ( !file.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename;
		return false;
	}
	
	if ( !file_h.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename_h;
		return false;
	}
	
	QTextStream stream_h( &file_h );
	stream_h << "#ifndef BNA_MD5_" << funcname.toUpper() << "_H\r\n";
	stream_h << "#define BNA_MD5_" << funcname.toUpper() << "_H\r\n\r\n";
	stream_h << "void " << funcname << "(";
	
	QTextStream stream( &file );
	stream << "#include \"" << fi.baseName() << ".h\"\r\n";
	stream << "void " << funcname << "(";
	for(unsigned int i = 0; i < m_nInput; i++){
		stream << "\r\n\tbool in" << i << ", ";
		stream_h << "\r\n\tbool in" << i << ", ";
	}
	
	for(unsigned  int i = 0; i < m_nOutput; i++){
		stream << "\r\n\tbool &out" << i;
		stream_h << "\r\n\tbool &out" << i;
		if(i < m_nOutput-1){
			stream << ", ";
			stream_h << ", ";
		}
	}
	stream << "\r\n) {\r\n";
	stream_h << "\r\n);\r\n\r\n";
	stream_h << "#endif //BNA_MD5_" << funcname.toUpper() << "_H\r\n";
	
	
	int nodes = m_nInput;
	for(int i = 0; i < m_vItems.size(); i++){
        QString sX = (m_vItems[i].getX() < m_nInput ? "in" : "node") + QString::number(m_vItems[i].getX());
        QString sY = (m_vItems[i].getY() < m_nInput ? "in" : "node") + QString::number(m_vItems[i].getY());
		QString sNode = "node" + QString::number(nodes); 
		stream << "\tbool " << sNode << " = " << sX << "|" << sY << ";\n";
		nodes++;
	}
	int out_nodes = nodes-m_nOutput;
	
	for(int i = out_nodes; i < nodes; i++){
		QString sOut = "out" + QString::number(i-out_nodes);
		QString sNode = "node" + QString::number(i);
		stream << "\t" << sOut << " = " << sNode << ";\n";
	}

	stream << "}\n";
	file.close();
	return true;
}

// ----------------------------------------------------------------

QByteArray BNA::exportToByteArray(){
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream << m_nInput << m_nOutput;
    for (int i = 0; i < m_vItems.size(); i++) {
        m_vItems[i].writeXYT(stream);
    }
    return data;
}

// ----------------------------------------------------------------

void BNA::importFromByteArray(QByteArray &data){
    QDataStream stream(&data, QIODevice::ReadOnly);
    m_vItems.clear();

    stream >> m_nInput;
    stream >> m_nOutput;

    while(!stream.atEnd()){
        BNAItem item;
        item.readXYT(stream);
        m_vItems.push_back(item);
    }
    normalize();
    return;
}

// ----------------------------------------------------------------

void BNA::generateRandomMutations(int nRandomCicles){
    QByteArray data = exportToByteArray();

    // random mutations data (exclude first 8 byte)
    int nOffset = 8;
    int nSize = data.size() - nOffset;
    char buf[1];
    for(int i = 0; i < nRandomCicles; i++){
        int x = qrand() % nSize;
        unsigned char ch = data.at(x + nOffset);
        buf[0] = ch + qrand();
        // std::cout << (int)ch << " -> " << (int)buf[0] << "\n";
        data = data.replace(x + nOffset, 1, buf);
    }
    importFromByteArray(data);
}

// ----------------------------------------------------------------

void BNA::appendRandomData(int nRandomCicles){
    QByteArray data = exportToByteArray();

    // random append data
    for(int i = 0; i < nRandomCicles; i++){
        // short x1
        data.append((char) qrand());
        data.append((char) qrand());
        // short y1
        data.append((char) qrand());
        data.append((char) qrand());
        // c0
        data.append((char) qrand());
    }
    importFromByteArray(data);
}

// ----------------------------------------------------------------

QJsonObject BNA::toJson(){
	
}

// ----------------------------------------------------------------

void BNA::clearResources(){
    for(int i = 0; i < m_vCalcExprs.size(); i++){
        delete m_vCalcExprs[i];
    }
    m_vCalcExprs.clear();

    for(int i = 0; i < m_vCalcVars.size(); i++){
        delete m_vCalcVars[i];
    }
    m_vCalcVars.clear();
    m_vCalcOutVars.clear();

}

// ----------------------------------------------------------------

void BNA::updateExprs(){
    clearResources();

    for(unsigned int i  = 0; i < m_nInput; i++){
        BNAVar *pVar = new BNAVar();
        pVar->val(false);
        pVar->name("in" + QString::number(i));
        m_vCalcVars.push_back(pVar);
    }

    int nItemsSize = m_vItems.size();
    for(int i = 0; i < m_vItems.size(); i++){
        int x = m_vItems[i].getX();
        int y = m_vItems[i].getY();
        int t = m_vItems[i].getT();
        BNAExpr *pExpr = new BNAExpr();
        pExpr->op1(m_vCalcVars[x]);
        pExpr->op2(m_vCalcVars[y]);
        pExpr->oper(m_vOpers[t]);
        BNAVar *pVar = new BNAVar();
        pVar->name("node" + QString::number(i));
        m_vCalcVars.push_back(pVar);
        pExpr->out(pVar);
        m_vCalcExprs.push_back(pExpr);
        if(nItemsSize - i <= m_nOutput){
            m_vCalcOutVars.push_back(pVar);
        }
    }
}

// ----------------------------------------------------------------

bool BNA::calc(const QVector<bool> &vInputs, int nOutput){
    // prepare calculate exprs

    if((unsigned int)vInputs.size() != m_nInput){
        qDebug() << "[ERROR] invalid input count";
        return false;
    }

    for(unsigned int i  = 0; i < m_nInput; i++){
        m_vCalcVars[i]->val(vInputs[i]);
    }

    for(int i = 0; i < m_vCalcExprs.size(); i++){
        m_vCalcExprs[i]->exec();
    }

    return m_vCalcOutVars[nOutput]->val();
}

// ----------------------------------------------------------------
