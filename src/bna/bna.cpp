#include <bna.h>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <iostream>
#include <QBuffer>
#include <QMap>
#include <QDir>
#include <iostream>
#include <cstring>
#include <QCryptographicHash>

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
    if(nSize == 0) {
        std::cout << "[FAIL] generateRandomMutations failed size " << nSize << "\n";
        return;
    }
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

BNABit BNA::calc(const QVector<BNABit> &vInputs, int nOutput){
    // prepare calculate exprs

    if((unsigned int)vInputs.size() != m_nInput){
        qDebug() << "[ERROR] invalid input count";
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

BNAMemoryItem::BNAMemoryItem() {
    // nothing
}

// ----------------------------------------------------------------

const QVector<BNABit> &BNAMemoryItem::inputToVectorBool(){
    if(m_vInput.size() != input.size()*8){
        // TODO max size
        BNAConvertArrayToVBool(input, m_vInput, input.size()*8);
    }
    return m_vInput;
}

// ----------------------------------------------------------------

const QVector<BNABit> &BNAMemoryItem::outputToVectorBool(){
    if(m_vOutput.size() != output.size()*8){
        // TODO max size
        BNAConvertArrayToVBool(output, m_vOutput, output.size()*8);
    }
    return m_vOutput;
}

// ----------------------------------------------------------------


BNAMemory::BNAMemory(){
    m_nInputSize = 55; // 55 bytes
    m_nOutputSize = 16; // 16 bytes of md5 hash
}

// ----------------------------------------------------------------

void BNAMemory::load(QString filename){
    m_vItems.clear();
    QFile file(filename);
    if (!file.exists()) {
        qDebug().noquote().nospace() << "RHMEMORY:  File did not exists: " << filename;
        return;
    }
    if ( !file.open(QIODevice::ReadOnly) ) {
        qDebug().noquote().nospace() << "RHMEMORY: Could not open file " << filename;
        return;
    }

    QDataStream stream( &file );
    char *pFileType = new char[9];
    std::memset(pFileType, 0, 9);
    int nReaded = stream.readRawData(pFileType, 8);
    if(nReaded > 0){
        if(QString(pFileType) != "RHMEMORY"){
            qDebug().noquote().nospace() << "RHMEMORY: File type did not match with RHMEMORY. " << filename;
            return;
        }
    }else{
        qDebug().noquote().nospace() << "RHMEMORY: Could not read file (1) " << filename;
        return;
    }

    stream >> m_nInputSize;
    stream >> m_nOutputSize;
    int nCount;
    stream >> nCount;
    for(int i = 0; i < nCount; i++){
        BNAMemoryItem memoryItem;
        stream >> memoryItem.input;
        stream >> memoryItem.output;
        m_vItems.push_back(memoryItem);
    }
};

// ----------------------------------------------------------------

void BNAMemory::save(QString filename){
    QFile file(filename);
    if (file.exists()) {
        file.remove();
    }
    if ( !file.open(QIODevice::WriteOnly) ) {
        qDebug().noquote().nospace() << "Could not write file: " << filename;
        return;
    }
    QDataStream stream( &file );
    stream.writeRawData("RHMEMORY", 8);
    stream << m_nInputSize << m_nOutputSize << m_vItems.size();
    for (int i = 0; i < m_vItems.size(); i++) {
        // QByteArray input(m_vItems[i].input);
        /*while(input.size() < m_nInputSize){
            input.append(char(0x00));
        }*/
        // stream.writeRawData(input.data(), input.size());
        // stream.writeRawData(m_vItems[i].output.data(), m_vItems[i].output.size());
        stream << m_vItems[i].input << m_vItems[i].output;
    }
    file.close();
};

// ----------------------------------------------------------------

int BNAMemory::size(){
    return m_vItems.size();
}

// ----------------------------------------------------------------

BNAMemoryItem BNAMemory::at(int i){
    return m_vItems[i];
}

// ----------------------------------------------------------------

void BNAMemory::printData(){
    qDebug().noquote().nospace() <<  " --- Reverse Hash Memory --- ";
    for (int i = 0; i < m_vItems.size(); i++) {
        qDebug().noquote().nospace() << m_vItems[i].input.toHex() << " => " << m_vItems[i].output.toHex();
    }
}

// ----------------------------------------------------------------

void BNAMemory::generateData(int nCount){
    m_vItems.clear();
    for(int i = 0; i < nCount; i++){
        BNAMemoryItem memoryItem;
        memoryItem.input.append(generateRandomString());
        memoryItem.output = QCryptographicHash::hash(memoryItem.input, QCryptographicHash::Md5);
        m_vItems.push_back(memoryItem);
    }
}

// ----------------------------------------------------------------

void BNAMemory::dataFrom(const QVector<QString> &vStrigns){
    m_vItems.clear();
    for(int i = 0; i < vStrigns.size(); i++){
        BNAMemoryItem memoryItem;
        memoryItem.input.append(vStrigns.at(i));
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
    int len = qrand() % (m_nInputSize) + 2;
    QString str = "";
    for (int i = 0; i < len; i++) {
        str += sAlphabet[qrand() % sAlphabet.length()];
    }
    return str;
}

// ----------------------------------------------------------------

BNAProject::BNAProject(){
    // nothing
}

// ----------------------------------------------------------------

bool BNAProject::open(QString sDirPath){
    QDir dir(sDirPath);
    if(!dir.exists()){
        std::cerr << "[ERROR] BNA Project " << sDirPath.toStdString() << " does not exists in this folder.\n";
        return false;
    }

    return true;
}

// ----------------------------------------------------------------


