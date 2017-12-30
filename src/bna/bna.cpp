#include "bna.h"
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

// -----------------------------------------------------------------
// BNA Var just contains boolean variable

BNAVar::BNAVar() {
   m_bVal = false;
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
}

// -----------------------------------------------------------------

void BNAExpr::op1(BNAVar *pVar1){
    m_pVar1 = pVar1;
}

// -----------------------------------------------------------------

void BNAExpr::op2(BNAVar *pVar2){
    m_pVar2 = pVar2;
}

// -----------------------------------------------------------------

void BNAExpr::out(BNAVar *pVarOut){
    m_pVarOut = pVarOut;
}

// -----------------------------------------------------------------

void BNAExpr::exec(){
    if(m_pVar1 == NULL){
        qDebug() << "[ERROR] m_pVar1 is NULL";
        return;
    }

    if(m_pVar2 == NULL){
        qDebug() << "[ERROR] m_pVar2 is NULL";
        return;
    }

    if(m_pVarOut == NULL){
        qDebug() << "[ERROR] m_pVarOut is NULL";
        return;
    }

    // TODO oper
    m_pVarOut->val(m_pVar1->val() ^ m_pVar2->val());
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

QString BNAOperXor::type(){ return QString("^"); }
bool BNAOperXor::calc(bool b1, bool b2){ return b1^b2; }

QString BNAOperAnd::type(){ return QString("&"); }
bool BNAOperAnd::calc(bool b1, bool b2){ return b1&b2; }

QString BNAOperOr::type(){ return QString("|"); }
bool BNAOperOr::calc(bool b1, bool b2){ return b1|b2; }

// -----------------------------------------------------------------

BNA::BNA(){
	m_nInput = 1; // Default
	m_nOutput = 1; // 16 bytes of md5 hash
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
};

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

void BNA::normalize(){
	int nodes = m_nInput;
	for(int i = 0; i < m_vItems.size(); i++){
        m_vItems[i].setX(m_vItems[i].getX() % nodes);
        m_vItems[i].setY(m_vItems[i].getY() % nodes);
        m_vItems[i].setT(m_vItems[i].getT() % 3);
		nodes++;
	}
    // prepare for calculations
    updateExprs();
}

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
	int nodes = m_nInput;

	for(int i = 0; i < m_vItems.size(); i++){
        QString sX = (m_vItems[i].getX() < m_nInput ? "in" : "node") + QString::number(m_vItems[i].getX());
        QString sY = (m_vItems[i].getY() < m_nInput ? "in" : "node") + QString::number(m_vItems[i].getY());
		QString sNode = "node" + QString::number(nodes); 
		stream << "\t" << sX << " -> " << sNode << ";\n";
		stream << "\t" << sY << " -> " << sNode << ";\n";
		nodes++;
	}
	int out_nodes = nodes-m_nOutput-1;
	for(int i = out_nodes; i < nodes; i++){
		QString sOut = "out" + QString::number(i-out_nodes);
		QString sNode = "node" + QString::number(i);
		stream << sNode << " -> " << sOut << ";\n";
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

void BNA::generateRandomMutations(){
	// TODO random changes
}

// ----------------------------------------------------------------

QJsonObject BNA::toJson(){
	
}

// ----------------------------------------------------------------

void BNA::updateExprs(){

    // TODO delete vars
    m_vCalcVars.clear();
    m_vCalcExprs.clear();
    m_vCalcOutVars.clear();

    for(unsigned int i  = 0; i < m_nInput; i++){
        BNAVar *pVar = new BNAVar();
        pVar->val(false);
        m_vCalcVars.push_back(pVar);
    }

    int nodes = m_nInput;
    for(int i = 0; i < m_vItems.size(); i++){
        int x = m_vItems[i].getX();
        int y = m_vItems[i].getY();
        BNAExpr *pExpr = new BNAExpr();
        pExpr->op1(m_vCalcVars[x]);
        pExpr->op2(m_vCalcVars[y]);
        BNAVar *pVar = new BNAVar();
        m_vCalcVars.push_back(pVar);
        pExpr->out(m_vCalcVars[nodes]);
        m_vCalcExprs.push_back(pExpr);
        nodes++;
    }

    int out_nodes = nodes-m_nOutput;

    for(int i = out_nodes; i < nodes; i++){
        m_vCalcOutVars.push_back(m_vCalcVars[i-out_nodes]);
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
