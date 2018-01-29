#include <bna.h>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <iostream>
#include <QBuffer>
#include <QMap>
#include <QTextStream>
#include <QDir>
#include <iostream>
#include <cstring>
#include <QJsonObject>
#include <QJsonDocument>
#include <QThread>

// -----------------------------------------------------------------
// function for convert hex string to array bna bit
struct BNABit4{
    BNABit4(BNABit b1, BNABit b2, BNABit b3, BNABit b4) : b1(b1), b2(b2), b3(b3), b4(b4) {}
    void appendToVector(QVector<BNABit> &vars){
        vars.push_back(b1);
        vars.push_back(b2);
        vars.push_back(b3);
        vars.push_back(b4);
    }
    BNABit b1;
    BNABit b2;
    BNABit b3;
    BNABit b4;
};

static QMap<QChar, BNABit4 *> gMapHEX;

void initMapHEX(){
    if(gMapHEX.size() == 0){
        gMapHEX['0'] = new BNABit4(B_0, B_0, B_0, B_0);
        gMapHEX['1'] = new BNABit4(B_0, B_0, B_0, B_1);
        gMapHEX['2'] = new BNABit4(B_0, B_0, B_1, B_0);
        gMapHEX['3'] = new BNABit4(B_0, B_0, B_1, B_1);
        gMapHEX['4'] = new BNABit4(B_0, B_1, B_0, B_0);
        gMapHEX['5'] = new BNABit4(B_0, B_1, B_0, B_1);
        gMapHEX['6'] = new BNABit4(B_0, B_1, B_1, B_0);
        gMapHEX['7'] = new BNABit4(B_0, B_1, B_1, B_1);
        gMapHEX['8'] = new BNABit4(B_1, B_0, B_0, B_0);
        gMapHEX['9'] = new BNABit4(B_1, B_0, B_0, B_1);
        gMapHEX['A'] = new BNABit4(B_1, B_0, B_1, B_0);
        gMapHEX['B'] = new BNABit4(B_1, B_0, B_1, B_1);
        gMapHEX['C'] = new BNABit4(B_1, B_1, B_0, B_0);
        gMapHEX['D'] = new BNABit4(B_1, B_1, B_0, B_1);
        gMapHEX['E'] = new BNABit4(B_1, B_1, B_1, B_0);
        gMapHEX['F'] = new BNABit4(B_1, B_1, B_1, B_1);
        gMapHEX['a'] = gMapHEX['A'];
        gMapHEX['b'] = gMapHEX['B'];
        gMapHEX['c'] = gMapHEX['C'];
        gMapHEX['d'] = gMapHEX['D'];
        gMapHEX['e'] = gMapHEX['E'];
        gMapHEX['f'] = gMapHEX['F'];
    }
}

void BNAConvertHEXStringToVBool(QString &in, QVector<BNABit> &vars, int size){
    if(size % 8 != 0){
        std::cerr << "[ERROR] Size must be % 8 == 0";
    }
    initMapHEX();
    vars.clear();
    for (int i = 0; i < in.length(); i++){
        QChar ch = in.at(i);
        if(gMapHEX.contains(ch)){
            gMapHEX[ch]->appendToVector(vars);
        }else{
            std::cerr << "[ERROR] Unknown hex char\n";
            vars.push_back(B_0);
            vars.push_back(B_0);
            vars.push_back(B_0);
            vars.push_back(B_0);
        }
    }

    while (vars.size() < size) {
        vars.push_back(B_0);
    }
}

// ----------------------------------------------------------------

QString BNAConvertCharToHexCode(unsigned char c) {
    if(c == 0) return "0";
    if(c == 1) return "1";
    if(c == 2) return "2";
    if(c == 3) return "3";
    if(c == 4) return "4";
    if(c == 5) return "5";
    if(c == 6) return "6";
    if(c == 7) return "7";
    if(c == 8) return "8";
    if(c == 9) return "9";
    if(c == 10) return "a";
    if(c == 11) return "b";
    if(c == 12) return "c";
    if(c == 13) return "d";
    if(c == 14) return "e";
    if(c == 15) return "f";
    return "x";
}

// ----------------------------------------------------------------

QString BNAConvertVBoolHEXString(QVector<BNABit> &vars) {
    QString result = "";
    unsigned char c = 0;
    for(int i = 0; i < vars.size(); i++){
        if(i % 4 == 0 && i != 0){
            result += BNAConvertCharToHexCode(c);
            c = 0x00;
        }
        c = (c << 1) | (vars[i] ? 0x01 : 0x00);
    }
    result += BNAConvertCharToHexCode(c);
    return result;
}

// -----------------------------------------------------------------

void BNAConvertArrayToVBool(QByteArray &in, QVector<BNABit> &vars, int size) {
    if(size % 8 != 0){
        std::cerr << "[error] Size must be % 8 == 0\n";
    }

    vars.clear();
    for (int i = 0; i < in.size(); i++)
    {
        unsigned char ch = in.at(i);
        for (int r = 7; r >= 0; r--) {
            if (vars.size() > size)
                return;
            unsigned char c = (int)ch >> r;
            c = c & 0x01;
            vars.push_back(c == 1 ? B_1 : B_0);
            if(c != 0 && c != 1){
                std::cerr << "[error] C must be 0 or 1\n";
            }
        }
    }

    while (vars.size() < size) {
        vars.push_back(B_0);
    }
}

// -----------------------------------------------------------------
// BNA Var just contains boolean variable

BNAVar::BNAVar() {
   m_bVal = B_0;
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

BNABit BNAVar::val(){
    return m_bVal;
}

// -----------------------------------------------------------------

void BNAVar::val(BNABit bVal){
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
    BNABit b1 = m_pVar1->val();
    BNABit b2 = m_pVar2->val();
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
    stream << m_nX;
    stream << m_nY;
    stream << m_cT;
}

// -----------------------------------------------------------------

QString BNAOperXor::type(){ return QString("XOR"); }

BNABit BNAOperXor::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 ^ c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------

QString BNAOperNotXor::type(){ return QString("NXOR"); }
BNABit BNAOperNotXor::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (!(c1 ^ c2)) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------

QString BNAOperAnd::type(){ return QString("AND"); }
BNABit BNAOperAnd::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 & c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------

QString BNAOperOr::type(){ return QString("OR"); }
BNABit BNAOperOr::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 | c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

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

unsigned int BNA::inputCount(){
    return m_nInput;
}

// ----------------------------------------------------------------

unsigned int BNA::outputCount(){
    return m_nOutput;
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
        std::cerr << "BNA:  File did not exists: " << filename.toStdString() << "\n";
		return false;
	}
	if (!file.open(QIODevice::ReadOnly) ) {
        std::cerr << "BNA: Could not open file " << filename.toStdString() << "\n";
		return false;
	}
    file.seek(0);
	QDataStream stream(&file);
    readFromStream(stream);
    file.close();
	return true;
}

// ----------------------------------------------------------------

bool BNA::save(QString filename){
	QFile file(filename);
	if (file.exists()) {
        if(!file.remove()){
            std::cerr << "Could not remove file: " << filename.toStdString() << "\n";
        }
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
        std::cerr << "Could not write file: " << filename.toStdString() << "\n";
		return false;
	}
    file.seek(0);
    QDataStream stream( &file );
    writeToStream(stream);
    file.close();
	return true;
}

// ----------------------------------------------------------------

void BNA::randomGenerate(int nInput, int nOutput, int nSize){
    clearResources();
	m_nInput = nInput;
	m_nOutput = nOutput;
	nSize = nSize + m_nInput + m_nOutput;
	for(int i = 0; i < nSize; i++){
        BNAItem *pItem = new BNAItem();
        pItem->setX(qrand());
        pItem->setY(qrand());
        pItem->setT(qrand());
        m_vItems.push_back(pItem);
	}
	normalize();
}

// ----------------------------------------------------------------

void BNA::normalize(){
    // normalize
    int nodes = m_nInput;
    for(int i = 0; i < m_vItems.size(); i++){
        m_vItems[i]->setX(m_vItems[i]->getX() % nodes);
        m_vItems[i]->setY(m_vItems[i]->getY() % nodes);
        m_vItems[i]->setT(m_vItems[i]->getT() % m_nOperSize);
        nodes++;
    }

    // prepare for calculations
    for(unsigned int i  = 0; i < m_nInput; i++){
        BNAVar *pVar = new BNAVar();
        pVar->val(B_0);
        pVar->name("in" + QString::number(i));
        m_vCalcVars.push_back(pVar);
    }

    int nItemsSize = m_vItems.size();
    for(int i = 0; i < m_vItems.size(); i++){
        int x = m_vItems[i]->getX();
        int y = m_vItems[i]->getY();
        int t = m_vItems[i]->getT();
        BNAExpr *pExpr = new BNAExpr();
        pExpr->op1(m_vCalcVars[x]);
        pExpr->op2(m_vCalcVars[y]);
        pExpr->oper(m_vOpers[t]);
        BNAVar *pVar = new BNAVar();
        pVar->name("node" + QString::number(i));
        m_vCalcVars.push_back(pVar);
        pExpr->out(pVar);
        m_vCalcExprs.push_back(pExpr);
        if(nItemsSize - i <= (int)m_nOutput){
            m_vCalcOutVars.push_back(pVar);
        }
    }
}

// ----------------------------------------------------------------

void BNA::compare(BNA &bna){
    if(bna.m_nInput != m_nInput){
        std::cout << "inputs not equals\n";
    }
    if(bna.m_nOutput != m_nOutput){
        std::cout << "outputs not equals\n";
    }
    if(bna.m_vItems.size() == m_vItems.size()){
        for(int i = 0; i < m_vItems.size(); i++){
            if(m_vItems[i]->getT() != bna.m_vItems[i]->getT()){
                std::cout << "\t T not equal in " << i << "\n";
            }

            if(m_vItems[i]->getX() != bna.m_vItems[i]->getX()){
                std::cout << "\t X not equal in " << i << "\n";
            }

            if(m_vItems[i]->getY() != bna.m_vItems[i]->getY()){
                std::cout << "\t Y not equal in " << i << "\n";
            }
        }
    }else{
        std::cout << "Item size not equals " << bna.m_vItems.size() << " != " << m_vItems.size() << " \n";
    }

    if(bna.m_vCalcExprs.size() == m_vCalcExprs.size()){
        for(int i = 0; i < m_vCalcExprs.size(); i++){
            if(m_vCalcExprs[i]->op1()->name() != bna.m_vCalcExprs[i]->op1()->name()){
                std::cout << "\t op1 not equal in " << i << "\n";
            }
            if(m_vCalcExprs[i]->op2()->name() != bna.m_vCalcExprs[i]->op2()->name()){
                std::cout << "\t op2 not equal in " << i << "\n";
            }
            if(m_vCalcExprs[i]->out()->name() != bna.m_vCalcExprs[i]->out()->name()){
                std::cout << "\t out not equal in " << i << "\n";
            }
            if(m_vCalcExprs[i]->oper()->type() != bna.m_vCalcExprs[i]->oper()->type()){
                std::cout << "\t oper not equal in " << i << "\n";
            }
        }
    }else{
        std::cout << "Exprs size not equals " << bna.m_vItems.size() << " != " << m_vItems.size() << " \n";
    }


}

// ----------------------------------------------------------------

bool BNA::exportToDot(QString filename, QString graphname){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
        std::cerr << "Could not write file: " << filename.toStdString() << "\n";
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
        if(nExprsSize - i <= (int)m_nOutput){
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
        std::cerr << "[ERROR]" << filename.toStdString() << " file must be have suffix 'cpp'\n";
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
        std::cerr << "Could not write file: " << filename.toStdString() << "\n";
		return false;
	}
	
	if ( !file_h.open(QIODevice::WriteOnly) ) {
        std::cerr << "Could not write file: " << filename_h.toStdString() << "\n";
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
        QString sX = (m_vItems[i]->getX() < m_nInput ? "in" : "node") + QString::number(m_vItems[i]->getX());
        QString sY = (m_vItems[i]->getY() < m_nInput ? "in" : "node") + QString::number(m_vItems[i]->getY());
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

void BNA::clearResources(){
    for(int i = 0; i < m_vItems.size(); i++){
        delete m_vItems[i];
    }
    m_vItems.clear();

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

void BNA::readFromStream(QDataStream &stream){
    clearResources();

    stream >> m_nInput;
    stream >> m_nOutput;

    while(!stream.atEnd()){
        BNAItem *pItem = new BNAItem();
        pItem->readXYT(stream);
        m_vItems.push_back(pItem);
    }
    normalize();
}

// ----------------------------------------------------------------

void BNA::writeToStream(QDataStream &stream){
    stream << m_nInput << m_nOutput;
    for (int i = 0; i < m_vItems.size(); i++) {
        m_vItems[i]->writeXYT(stream);
    }
}

// ----------------------------------------------------------------

QByteArray BNA::exportToByteArray(){
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    writeToStream(stream);
    return data;
}

// ----------------------------------------------------------------

void BNA::importFromByteArray(QByteArray data){
    clearResources();
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QIODevice::ReadOnly);
    buffer.seek(0);
    QDataStream stream(&buffer);
    readFromStream(stream);
    return;
}

// ----------------------------------------------------------------

void BNA::generateRandomMutations(int nRandomCicles){
    QByteArray data = exportToByteArray();

    // random mutations data (exclude first 8 byte)
    int nOffset = 8;
    int nSize = data.size() - nOffset;
    if(nSize <= 0) {
        std::cout << "[FAIL] generateRandomMutations failed size " << nSize << "\n";
        return;
    }
    char buf[1];
    for(int i = 0; i < nRandomCicles; i++){
        int x = qrand() % nSize;
        unsigned char ch = data.data()[x + nOffset];
        buf[0] = ch + qrand();
        // std::cout << (int)ch << " -> " << (int)buf[0] << "\n";
        data.data()[x + nOffset] = buf[0];
        // data = data.replace(x + nOffset, 1, buf);
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

BNABit BNA::calc(const QVector<BNABit> &vInputs, int nOutput){
    // prepare calculate exprs

    if((unsigned int)vInputs.size() != m_nInput){
        std::cerr << "[ERROR] invalid input count " << vInputs.size() << "(Expected: " << m_nInput << ") \n";
        return B_0;
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

BNAMemoryItem::BNAMemoryItem(int nInputBits, int nOutputBits) {
    m_nInputBits = nInputBits;
    m_nOutputBits = nOutputBits;
}

// ----------------------------------------------------------------

const QVector<BNABit> &BNAMemoryItem::inputToVectorBool(){
    if(m_vInput.size() == 0){
        BNAConvertArrayToVBool(input, m_vInput, m_nInputBits);
    }
    return m_vInput;
}

// ----------------------------------------------------------------

const QVector<BNABit> &BNAMemoryItem::outputToVectorBool(){
    if(m_vOutput.size() == 0){
        BNAConvertArrayToVBool(output, m_vOutput, m_nOutputBits);
    }
    return m_vOutput;
}

// ----------------------------------------------------------------

BNAMemory::BNAMemory(int nInputBits, int nOutputBits){
    m_nInputBits = nInputBits;
    m_nOutputBits = nOutputBits;
}

// ----------------------------------------------------------------

void BNAMemory::load(QString filename){

    QFile file(filename);
    if (!file.exists()) {
        std::cerr << "BNAMEMORY:  File did not exists: " << filename.toStdString() << "\n";
        return;
    }
    if ( !file.open(QIODevice::ReadOnly) ) {
        std::cerr << "BNAMEMORY: Could not open file " << filename.toStdString() << "\n";
        return;
    }

    m_vItems.clear();

    QDataStream stream( &file );

    int nInputBits = 0;
    int nOutputBits = 0;
    stream >> nInputBits;
    stream >> nOutputBits;
    if(nInputBits != m_nInputBits){
        std::cerr << "Invalid input bits in memory file. Filename: "  << filename.toStdString() << " \n";
        return;
    }

    if(nOutputBits != m_nOutputBits){
        std::cerr << "Invalid output bits in memory file.\n";
        return;
    }

    int nCount;
    stream >> nCount;
    for(int i = 0; i < nCount; i++){
        BNAMemoryItem *pBNAMemoryItem = new BNAMemoryItem(nInputBits, nOutputBits);
        stream >> pBNAMemoryItem->input;
        stream >> pBNAMemoryItem->output;
        m_vItems.push_back(pBNAMemoryItem);
    }
};

// ----------------------------------------------------------------

void BNAMemory::save(QString filename){
    QFile file(filename);
    if (file.exists()) {
        file.remove();
    }
    if ( !file.open(QIODevice::WriteOnly) ) {
        std::cerr << "Could not write file: " << filename.toStdString() << "\n";
        return;
    }
    QDataStream stream( &file );
    stream << m_nInputBits << m_nOutputBits << m_vItems.size();
    for (int i = 0; i < m_vItems.size(); i++) {
        stream << m_vItems[i]->input << m_vItems[i]->output;
    }
    file.close();
};

// ----------------------------------------------------------------

int BNAMemory::size(){
    return m_vItems.size();
}

// ----------------------------------------------------------------

BNAMemoryItem* BNAMemory::at(int i){
    return m_vItems[i];
}

// ----------------------------------------------------------------

BNAMemoryItem * BNAMemory::createItem(){
    return new BNAMemoryItem(m_nInputBits, m_nOutputBits);
}

// ----------------------------------------------------------------

void BNAMemory::append(BNAMemoryItem *pBNAMemoryItem){
    m_vItems.push_back(pBNAMemoryItem);
}

// ----------------------------------------------------------------

void BNAMemory::printData(){
    std::cerr <<  " --- BNA Memory --- \n";
    for (int i = 0; i < m_vItems.size(); i++) {
        std::cerr << m_vItems[i]->input.toHex().toStdString() << " => " << m_vItems[i]->output.toHex().toStdString() << "\n";
    }
}

// ----------------------------------------------------------------

/*void BNAMemory::generateData(int nCount){
    m_vItems.clear();
    for(int i = 0; i < nCount; i++){
        BNAMemoryItem memoryItem;
        memoryItem.input.append(generateRandomString());
        memoryItem.output = QCryptographicHash::hash(memoryItem.input, QCryptographicHash::Md5);
        m_vItems.push_back(memoryItem);
    }
}

// ----------------------------------------------------------------

QString BNAMemory::alphabet() {
    return "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890[]{}:,.<>/?\"'\\*&^%$#!-+=";
}

// ----------------------------------------------------------------

QString BNAMemory::generateRandomString(){
    QString sAlphabet = alphabet();
    int len = qrand() % (m_nInputBits) + 2;
    QString str = "";
    for (int i = 0; i < len; i++) {
        str += sAlphabet[qrand() % sAlphabet.length()];
    }
    return str;
}
*/
// ----------------------------------------------------------------

BNAProject::BNAProject(){
    m_nInputBits = 1;
    m_nOutputBits = 1;
    m_nDefaultCountNodes = 5;
}

// ----------------------------------------------------------------

void BNAProject::setInputBits(int nInputBits){
    m_nInputBits = nInputBits;
}

// ----------------------------------------------------------------

int BNAProject::getInputBits(){
    return m_nInputBits;
}

// ----------------------------------------------------------------

void BNAProject::setOutputBits(int nOutputBits){
    m_nOutputBits = nOutputBits;
}

// ----------------------------------------------------------------

int BNAProject::getOutputBits(){
    return m_nOutputBits;
}

// ----------------------------------------------------------------

void BNAProject::setDefaultCountNodes(int nDefaultCountNodes){
    m_nDefaultCountNodes = nDefaultCountNodes;
}

// ----------------------------------------------------------------

bool BNAProject::open(QString sDirPath){
    QDir dir(sDirPath);
    if(!dir.exists()){
        std::cerr << "[ERROR] BNA Project " << sDirPath.toStdString() << " does not exists in this folder.\n";
        return false;
    }
    m_sDirPath = sDirPath;
    loadProjFile();
    m_pBNAMemory = new BNAMemory(m_nInputBits, m_nOutputBits);
    m_pBNAMemory->load(m_sDirPath + "/" + m_sMemoryFileName);

    for (int bitid = 0; bitid < m_nOutputBits; bitid++) {
        QString m_sBitid = prepareName(bitid);
        QString subdir = prepareSubdir(bitid);
        QString m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".bna";
        BNA *pBNA = new BNA();
        pBNA->load(m_sFilename);
        m_mBNA[bitid] = pBNA;
        m_mapResults[bitid] = loadResult(bitid);
    }

    return true;
}

// ----------------------------------------------------------------

QString BNAProject::prepareName(int bitid) {
    return QString::number(bitid).rightJustified(3, '0');
}

// ----------------------------------------------------------------

QString BNAProject::prepareSubdir(int bitid){
    QString m_sBitid = QString::number(bitid).rightJustified(3, '0');
    return m_sBitid[0] + "/" + m_sBitid[1] + "/" + m_sBitid[2];
}

// ----------------------------------------------------------------

bool BNAProject::create(QString sDirPath){
    QDir dir(sDirPath);
    if(dir.exists()){
        std::cerr << "[ERROR] BNA Project " << sDirPath.toStdString() << " already exists.\n";
        return false;
    }
    QDir curr(".");
    curr.mkpath(sDirPath);
    m_sDirPath = sDirPath;

    for (int bitid = 0; bitid < m_nOutputBits; bitid++) {
        QString m_sBitid = prepareName(bitid);
        QString subdir = prepareSubdir(bitid);
        curr.mkpath(m_sDirPath + "/" + subdir);
        QString m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".bna";

        QFile file(m_sFilename);
        if(!file.exists()){
            BNA *pBNA = new BNA();
            pBNA->randomGenerate(m_nInputBits,1,m_nDefaultCountNodes);
            pBNA->save(m_sFilename);
            m_mBNA[bitid] = pBNA;
        }
        m_mapResults[bitid] = 0;
        saveResult(bitid, 0);
    }
    m_pBNAMemory = new BNAMemory(m_nInputBits, m_nOutputBits);
    m_sMemoryFileName = "data.bnamemory";
    m_pBNAMemory->save(m_sDirPath + "/" + m_sMemoryFileName);
    saveProjFile();

    return true;
}

// ----------------------------------------------------------------

void BNAProject::saveProjFile(){
    QJsonObject proj;
    proj["input_bits"] = m_nInputBits;
    proj["output_bits"] = m_nOutputBits;
    proj["default_count_nodes"] = m_nDefaultCountNodes;
    proj["memory"] = m_sMemoryFileName;

    QJsonDocument doc(proj);
    QString sProj = doc.toJson(QJsonDocument::Indented);
    QString sFilename = m_sDirPath + "/bnaproject.json";
    QFile file(sFilename);
    if (file.exists()) {
        file.remove();
    }
    if ( !file.open(QIODevice::WriteOnly) ) {
        std::cerr << "[ERROR] Could not write file: " << sFilename.toStdString() << "\n";
        return;
    }

    QTextStream stream( &file );
    stream << sProj;
    file.close();
}

// ----------------------------------------------------------------

void BNAProject::loadProjFile(){
    QString sFilename = m_sDirPath + "/bnaproject.json";
    QFile file(sFilename);
    if (!file.exists()) {
        std::cerr << "[ERROR] Did not found file: " << sFilename.toStdString() << "\n";
        return;
    }

    if ( !file.open(QIODevice::ReadOnly) ) {
        std::cerr << "[ERROR] Could not read file: " << sFilename.toStdString() << "\n";
        return;
    }

    QTextStream stream( &file );
    QString sProj = stream.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(sProj.toUtf8());
    QJsonObject proj = doc.object();
    m_nInputBits = proj["input_bits"].toInt();
    m_nOutputBits = proj["output_bits"].toInt();
    m_nDefaultCountNodes = proj["default_count_nodes"].toInt();
    m_sMemoryFileName = proj["memory"].toString();
}

// ----------------------------------------------------------------

BNAMemory *BNAProject::getBNAMemory(){
    return m_pBNAMemory;
}

// ----------------------------------------------------------------

void BNAProject::saveBNAMemory(){
    m_pBNAMemory->save(m_sDirPath + "/" + m_sMemoryFileName);
}

// ----------------------------------------------------------------

int BNAProject::calculate(int bitid, bool bEnableSleep){

    if(bitid > m_nOutputBits){
        std::cerr << "Invalid bitid max possible" << (m_nOutputBits-1) << "\n";
        return 0;
    }

    if(!m_mBNA.contains(bitid)){
        std::cerr << "Not found bitid in array\n";
        return 0;
    }

    int nResult = 0;
    BNA *pBNA = m_mBNA[bitid];
    int nMemorySize = m_pBNAMemory->size();
    for(int i = 0; i < nMemorySize; i++){
        BNAMemoryItem *pBNAMemoryItem = m_pBNAMemory->at(i);
        BNABit bResult = pBNA->calc(pBNAMemoryItem->inputToVectorBool(), 0);
        if(pBNAMemoryItem->outputToVectorBool()[bitid] == bResult){
            nResult++;
        }

        if(bEnableSleep && i > 0 && i % 1000 == 0){
            QThread::sleep(1);
        }
    }
    return nResult;
}

// ----------------------------------------------------------------

void BNAProject::saveResult(int bitid, int nResult){
	QString m_sBitid = prepareName(bitid);
	QString subdir = prepareSubdir(bitid);
	QString m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".result";
	
	QFile file(m_sFilename);
	if (file.exists()) {
		file.remove();
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
		std::cout << "BNAProject/savePersent: Could not write file: " << m_sFilename.toStdString() << "\n";
		return;
	}
	QDataStream stream( &file );
    stream << nResult;
    m_mapResults[bitid] = nResult;
	file.close();
}

// ----------------------------------------------------------------

int BNAProject::loadResult(int bitid){
	QString m_sBitid = prepareName(bitid);
	QString subdir = prepareSubdir(bitid);
	QString m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".result";
	
    int nResult = 0;
	// load persent
	QFile file(m_sFilename);
	if (!file.exists()) {
		std::cout << "BNAProject/loadPersent:  File did not exists: " << m_sFilename.toStdString() << "\n";
	}else{
		if (!file.open(QIODevice::ReadOnly) ) {
			std::cout << "BNAProject/loadPersent: Could not open file " << m_sFilename.toStdString() << "\n";
		}else{
			QDataStream stream(&file);
            stream >> nResult;
		}
	}
    m_mapResults[bitid] = nResult;
    return nResult;
}
        
// ----------------------------------------------------------------

QMap<int,int> &BNAProject::getResults(){
    return m_mapResults;
}

// ----------------------------------------------------------------

BNA *BNAProject::getBNA(int bitid){
    if(bitid > m_nOutputBits){
        std::cerr << "Invalid bitid max possible" << (m_nOutputBits-1) << "\n";
        return NULL;
    }

    if(!m_mBNA.contains(bitid)){
        std::cerr << "Not found bitid in array\n";
        return NULL;
    }

    return m_mBNA[bitid];
}

// ----------------------------------------------------------------

void BNAProject::saveBNA(int bitid){
    if(bitid > m_nOutputBits){
        std::cerr << "Invalid bitid max possible" << (m_nOutputBits-1) << "\n";
        return;
    }

    if(!m_mBNA.contains(bitid)){
        std::cerr << "Not found bitid in array\n";
        return;
    }

    QString m_sBitid = prepareName(bitid);
    QString subdir = prepareSubdir(bitid);
    QString m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".bna";
    m_mBNA[bitid]->save(m_sFilename);
}

// ----------------------------------------------------------------
