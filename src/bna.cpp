#include <bna.h>
#include <iostream>
#include <iostream>
#include <cstring>
#include <wsjcpp_core.h>

// -----------------------------------------------------------------
// function for convert hex string to array bna bit

struct BNABit4 {
    BNABit4(BNABit b1, BNABit b2, BNABit b3, BNABit b4) : b1(b1), b2(b2), b3(b3), b4(b4) {}
    void appendToVector(std::vector<BNABit> &vars){
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

static std::map<char, BNABit4 *> gMapHEX;

void initMapHEX(){
    if (gMapHEX.size() == 0) {
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

void BNAConvertHEXStringToVBool(std::string &in, std::vector<BNABit> &vars, int size){
    if (size % 8 != 0) {
        std::cerr << "[ERROR] Size must be % 8 == 0";
    }
    initMapHEX();
    vars.clear();
    for (int i = 0; i < in.length(); i++){
        char ch = in.at(i);
        std::map<char, BNABit4 *>::iterator it;
        it = gMapHEX.find(ch);
        if(it != gMapHEX.end()){
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

std::string BNAConvertCharToHexCode(unsigned char c) {
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

static std::map<char, std::string> gMapHEX2BIN;

void initMapHEX2BIN(){
    if (gMapHEX2BIN.size() == 0) {
        gMapHEX2BIN['0'] = "0000";
        gMapHEX2BIN['1'] = "0001";
        gMapHEX2BIN['2'] = "0010";
        gMapHEX2BIN['3'] = "0011";
        gMapHEX2BIN['4'] = "0100";
        gMapHEX2BIN['5'] = "0101";
        gMapHEX2BIN['6'] = "0110";
        gMapHEX2BIN['7'] = "0111";
        gMapHEX2BIN['8'] = "1000";
        gMapHEX2BIN['9'] = "1001";
        gMapHEX2BIN['A'] = "1010";
        gMapHEX2BIN['B'] = "1011";
        gMapHEX2BIN['C'] = "1100";
        gMapHEX2BIN['D'] = "1101";
        gMapHEX2BIN['E'] = "1110";
        gMapHEX2BIN['F'] = "1111";
        gMapHEX2BIN['a'] = gMapHEX2BIN['A'];
        gMapHEX2BIN['b'] = gMapHEX2BIN['B'];
        gMapHEX2BIN['c'] = gMapHEX2BIN['C'];
        gMapHEX2BIN['d'] = gMapHEX2BIN['D'];
        gMapHEX2BIN['e'] = gMapHEX2BIN['E'];
        gMapHEX2BIN['f'] = gMapHEX2BIN['F'];
    }
}

// ----------------------------------------------------------------

std::string BNAConvertHexToBin(std::string sHex){
    initMapHEX2BIN();
    std::string sResult = "";
    for (int i = 0; i < sHex.length(); i++){
        char ch = sHex.at(i);
        std::map<char, BNABit4 *>::iterator it;
        it = gMapHEX.find(ch);
        if (it != gMapHEX.end()) {
            sResult += gMapHEX2BIN[ch];
        } else {
            std::cerr << "[ERROR] Unknown hex char\n";
        }
    }
    return sResult;
}

// ----------------------------------------------------------------

std::string BNAConvertBinToHex(std::string sBin) {
    std::string result = "";
    unsigned char c = 0;
    for(int i = 0; i < sBin.length(); i++){
        char ch = sBin.at(i);
        if(i % 4 == 0 && i != 0){
            result += BNAConvertCharToHexCode(c);
            c = 0x00;
        }
        c = (c << 1) | (ch == '1' ? 0x01 : 0x00);
    }
    result += BNAConvertCharToHexCode(c);
    return result;
}

// ----------------------------------------------------------------

int BNACalculateBinDistance(std::string sBin1, std::string sBin2){
    if(sBin1.length() != sBin2.length()){
        std::cerr << "[ERROR] Different length \n";
        return 0;
    }

    if(sBin1.length() == 0 || sBin2.length() == 0){
        std::cerr << "[ERROR] Length is 0 - wrong \n";
        return 0;
    }
    int nDistance = 0;
    for (int i = 0; i < sBin1.length(); i++){
        char ch1 = sBin1.at(i);
        char ch2 = sBin2.at(i);
        if(ch1 == ch2){
            nDistance++;
        }
    }
    return nDistance;
}

// ----------------------------------------------------------------

std::string BNAConvertVBoolHEXString(std::vector<BNABit> &vars) {
    std::string result = "";
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

// void BNAConvertArrayToVBool(QByteArray &in, std::vector<BNABit> &vars, int size) {
//     if(size % 8 != 0){
//         std::cerr << "[error] Size must be % 8 == 0\n";
//     }

//     vars.clear();
//     for (int i = 0; i < in.size(); i++)
//     {
//         unsigned char ch = in.at(i);
//         for (int r = 7; r >= 0; r--) {
//             if (vars.size() > size)
//                 return;
//             unsigned char c = (int)ch >> r;
//             c = c & 0x01;
//             vars.push_back(c == 1 ? B_1 : B_0);
//             if(c != 0 && c != 1){
//                 std::cerr << "[error] C must be 0 or 1\n";
//             }
//         }
//     }

//     while (vars.size() < size) {
//         vars.push_back(B_0);
//     }
// }

// -----------------------------------------------------------------

// QByteArray BNATryBrutFast1(const QByteArray &arrReversedText, const std::string &sMd5ExpectedHex){
//     // TODO create input funtor
//     QByteArray result_md5 = QCryptographicHash::hash(arrReversedText, QCryptographicHash::Md5);
//     std::string sReverseHash1 = std::string(result_md5.toHex());

//     std::string sMd5ExpectedHex_bin = BNAConvertHexToBin(sMd5ExpectedHex);
//     std::string sReverseHash1_bin = BNAConvertHexToBin(sReverseHash1);
//     int nPrevDistance = BNACalculateBinDistance(sReverseHash1_bin, sMd5ExpectedHex_bin);
//     std::string sPrevText = BNAConvertHexToBin(arrReversedText.toHex());

//     bool bExists = true;
//     int nRound = 0;
//     while(bExists){
//         bExists = false;
//         std::cout << "Round(1) #" << nRound << "\n";
//         nRound++;
//         std::string sTmpPrevText = std::string(sPrevText);
//         for(int i = 0; i < sTmpPrevText.length(); i++){
//             char ch = sTmpPrevText.at(i);
//             ch = ch == '1' ? '0' : '1';
//             sTmpPrevText[i] = ch;
//             std::string sTmpHex = BNAConvertBinToHex(sTmpPrevText);
//             QByteArray bTmpText =  QByteArray::fromHex(sTmpHex.toLatin1());
//             QByteArray sTmpMD5 = QCryptographicHash::hash(bTmpText, QCryptographicHash::Md5);
//             std::string sTmpMD5_hex = std::string(sTmpMD5.toHex());
//             std::string sTmpMD5_bin = BNAConvertHexToBin(sTmpMD5_hex);
//             int nNewDistance = BNACalculateBinDistance(sMd5ExpectedHex_bin, sTmpMD5_bin);
//             if(nNewDistance > nPrevDistance){
//                 sPrevText = std::string(sTmpPrevText);
//                 nPrevDistance = nNewDistance;
//                 bExists = true;
//             }
//             ch = ch == '1' ? '0' : '1';
//             sTmpPrevText[i] = ch;
//         }
//     }

//     QByteArray bTmpText =  QByteArray::fromHex(BNAConvertBinToHex(sPrevText).toLatin1());
//     return bTmpText;

// }

// // -----------------------------------------------------------------

// QByteArray BNATryBrutFast2(const QByteArray &arrReversedText, const std::string &sMd5ExpectedHex){
//     // TODO create input funtor
//     QByteArray result_md5 = QCryptographicHash::hash(arrReversedText, QCryptographicHash::Md5);
//     std::string sReverseHash1 = std::string(result_md5.toHex());

//     std::string sMd5ExpectedHex_bin = BNAConvertHexToBin(sMd5ExpectedHex);
//     std::string sReverseHash1_bin = BNAConvertHexToBin(sReverseHash1);
//     int nPrevDistance = BNACalculateBinDistance(sReverseHash1_bin, sMd5ExpectedHex_bin);
//     std::string sPrevText = BNAConvertHexToBin(arrReversedText.toHex());

//     bool bExists = true;
//     int nRound = 0;
//     while(bExists){
//         bExists = false;
//         std::cout << "Round(2) #" << nRound << "\n";
//         nRound++;
//         std::string sTmpPrevText = std::string(sPrevText);
//         for(int x = 0; x < sTmpPrevText.length(); x++){
//             for(int y = 0; y < sTmpPrevText.length(); y++){
//                 if(x == y) continue;
//                 char chX = sTmpPrevText.at(x);
//                 char chY = sTmpPrevText.at(y);
//                 chX = chX == '1' ? '0' : '1';
//                 chY = chY == '1' ? '0' : '1';
//                 sTmpPrevText[x] = chX;
//                 sTmpPrevText[y] = chY;
//                 std::string sTmpHex = BNAConvertBinToHex(sTmpPrevText);
//                 QByteArray bTmpText =  QByteArray::fromHex(sTmpHex.toLatin1());
//                 QByteArray sTmpMD5 = QCryptographicHash::hash(bTmpText, QCryptographicHash::Md5);
//                 std::string sTmpMD5_hex = std::string(sTmpMD5.toHex());
//                 std::string sTmpMD5_bin = BNAConvertHexToBin(sTmpMD5_hex);
//                 int nNewDistance = BNACalculateBinDistance(sMd5ExpectedHex_bin, sTmpMD5_bin);
//                 if(nNewDistance > nPrevDistance){
//                     sPrevText = std::string(sTmpPrevText);
//                     nPrevDistance = nNewDistance;
//                     bExists = true;
//                 }
//                 chX = chX == '1' ? '0' : '1';
//                 chY = chY == '1' ? '0' : '1';
//                 sTmpPrevText[x] = chX;
//                 sTmpPrevText[y] = chY;
//             }
//         }
//     }

//     QByteArray bTmpText =  QByteArray::fromHex(BNAConvertBinToHex(sPrevText).toLatin1());
//     return bTmpText;
// }

// -----------------------------------------------------------------
// BNA Var just contains boolean variable

BNAVar::BNAVar() {
   m_bVal = B_0;
   m_sName = "";
}

// -----------------------------------------------------------------

void BNAVar::name(std::string name){
    m_sName = name;
}

// -----------------------------------------------------------------

std::string BNAVar::name(){
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

BNAItem::BNAItem(unsigned short x, unsigned short y, const std::string &sOperationType){
    m_nX = x;
    m_nY = y;
    m_sOperationType = sOperationType;
}

// -----------------------------------------------------------------

BNAItem::BNAItem() {
    m_nX = 0;
    m_nY = 0;
    m_sOperationType = "";
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

std::string BNAItem::getOperationType(){
    return m_sOperationType;
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

void BNAItem::setOperationType(const std::string &sOperationType){
    m_sOperationType = sOperationType;
}

// -----------------------------------------------------------------

// void BNAItem::readXYT(QDataStream &stream){
//     stream >> m_nX;
//     stream >> m_nY;
//     stream >> m_cT;
// }

// -----------------------------------------------------------------

void BNAItem::writeXYT(std::ofstream &file){
    file << " item " << m_nX << " " << m_nY << " " << m_sOperationType;
}

// -----------------------------------------------------------------

std::string BNAOperXor::type(){ return std::string("XOR"); }

BNABit BNAOperXor::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 ^ c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------

std::string BNAOperNotXor::type(){ return std::string("NXOR"); }
BNABit BNAOperNotXor::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (!(c1 ^ c2)) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------

std::string BNAOperAnd::type(){ return std::string("AND"); }
BNABit BNAOperAnd::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 & c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------

std::string BNAOperOr::type(){ return std::string("OR"); }
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
    registryOperationType(new BNAOperXor());
    registryOperationType(new BNAOperNotXor());
    registryOperationType(new BNAOperAnd());
    registryOperationType(new BNAOperOr());
    m_nOperSize = m_vOperationList.size();
    TAG = "BNA";
    m_nBnaVersion = 1;
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

BNA::~BNA() {
    m_vOperations.clear();
    m_vOperationList.clear();
    m_vItems.clear();
    clearResources();
}

// ----------------------------------------------------------------

bool BNA::load(std::string filename){
    WsjcppLog::err(TAG, "TODO load");

    m_vOperations.clear();
    m_vOperationList.clear();
    m_vItems.clear();
    clearResources();
    return false;
	/*QFile file(filename);
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
    file.close();*/
	// return true;
}

// ----------------------------------------------------------------

bool BNA::save(const std::string &sFilename){
    WsjcppLog::err(TAG, "TODO save");
    std::string sFilename0 = sFilename + ".bna";
    if (WsjcppCore::fileExists(sFilename0)) {
        if (!WsjcppCore::removeFile(sFilename0)) {
            WsjcppLog::err(TAG, "save: could not remove file: " + sFilename0);
            return false;
        }
    }
    std::ofstream file;
    file.open(sFilename0, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        WsjcppLog::err(TAG, "save: could not open file to write: " + sFilename0);
		return false;
    }
    writeToFileBna(file);
    file.close();
	return true;
}

// ----------------------------------------------------------------

void BNA::randomGenerate(int nInput, int nOutput, int nSize){
    clearResources();
	m_nInput = nInput;
	m_nOutput = nOutput;
	nSize = nSize + m_nInput + m_nOutput;
	for (int i = 0; i < nSize; i++) {
        BNAItem *pItem = new BNAItem();
        pItem->setX(rand());
        pItem->setY(rand());
        int nOper = rand() % m_nOperSize;
        pItem->setOperationType(m_vOperationList[nOper]->type());
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
        m_vItems[i]->setOperationType(m_vItems[i]->getOperationType());
        nodes++;
    }

    // prepare for calculations
    for(unsigned int i  = 0; i < m_nInput; i++){
        BNAVar *pVar = new BNAVar();
        pVar->val(B_0);
        pVar->name("in" + std::to_string(i));
        m_vCalcVars.push_back(pVar);
    }

    int nItemsSize = m_vItems.size();
    for(int i = 0; i < m_vItems.size(); i++){
        int x = m_vItems[i]->getX();
        int y = m_vItems[i]->getY();
        std::string sOperationType = m_vItems[i]->getOperationType();
        BNAExpr *pExpr = new BNAExpr();
        pExpr->op1(m_vCalcVars[x]);
        pExpr->op2(m_vCalcVars[y]);
        pExpr->oper(m_vOperations[sOperationType]);
        BNAVar *pVar = new BNAVar();
        pVar->name("node" + std::to_string(i));
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
            if(m_vItems[i]->getOperationType() != bna.m_vItems[i]->getOperationType()){
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

bool BNA::exportToDot(std::string filename, std::string graphname){
    WsjcppLog::err(TAG, "TODO exportToCpp");

	/*QFile file(filename);
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
            stream << "\t" << pExpr->out()->name() << " -> out" << std::string::number(nExprsSize - i) << ";\n";
        }
    }

    stream << "}\n";
	file.close();*/
	return true;
}

// ----------------------------------------------------------------

bool BNA::exportToCpp(std::string filename, std::string funcname){
    WsjcppLog::err(TAG, "TODO exportToCpp");
	/*QFile file(filename);
	QFileInfo fi(filename);
	if(fi.suffix() != "cpp"){
        std::cerr << "[ERROR]" << filename.toStdString() << " file must be have suffix 'cpp'\n";
		return false;
	}
	
	std::string filename_h = filename.left(filename.length() - 3);
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
        std::string sX = (m_vItems[i]->getX() < m_nInput ? "in" : "node") + std::to_string(m_vItems[i]->getX());
        std::string sY = (m_vItems[i]->getY() < m_nInput ? "in" : "node") + std::to_string(m_vItems[i]->getY());
		std::string sNode = "node" + std::string::number(nodes); 
		stream << "\tbool " << sNode << " = " << sX << "|" << sY << ";\n";
		nodes++;
	}
	int out_nodes = nodes-m_nOutput;
	
	for(int i = out_nodes; i < nodes; i++){
		std::string sOut = "out" + std::to_string(i-out_nodes);
		std::string sNode = "node" + std::to_string(i);
		stream << "\t" << sOut << " = " << sNode << ";\n";
	}

	stream << "}\n";
	file.close();*/
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

// void BNA::readFromStream(QDataStream &stream){
//     clearResources();

//     stream >> m_nInput;
//     stream >> m_nOutput;

//     while(!stream.atEnd()){
//         BNAItem *pItem = new BNAItem();
//         pItem->readXYT(stream);
//         m_vItems.push_back(pItem);
//     }
//     normalize();
// }

// ----------------------------------------------------------------

void BNA::writeToFileBna(std::ofstream &file){
    // basic information about file
    file << "BNA"; 
    file << " version " << m_nBnaVersion;
    file << " input " << m_nInput;
    file << " output " << m_nOutput;
    for (int i = 0; i < m_vItems.size(); i++) {
        m_vItems[i]->writeXYT(file);
    }
}

// ----------------------------------------------------------------

bool BNA::registryOperationType(IBNAOper *pOper) {
    // TODO check aredy registered
    m_vOperations[pOper->type()] = pOper;
    m_vOperationList.push_back(pOper);
    return true;
}

// ----------------------------------------------------------------

// QByteArray BNA::exportToByteArray(){
//     QByteArray data;
//     QDataStream stream( &data, QIODevice::WriteOnly );
//     writeToStream(stream);
//     return data;
// }

// ----------------------------------------------------------------

// void BNA::importFromByteArray(QByteArray data){
//     clearResources();
//     QBuffer buffer;
//     buffer.setData(data);
//     buffer.open(QIODevice::ReadOnly);
//     buffer.seek(0);
//     QDataStream stream(&buffer);
//     readFromStream(stream);
//     return;
// }

// ----------------------------------------------------------------

void BNA::generateRandomMutations(int nRandomCicles){
    WsjcppLog::err(TAG, "TODO generateRandomMutations");

    /*QByteArray data = exportToByteArray();

    // random mutations data (exclude first 8 byte)
    int nOffset = 8;
    int nSize = data.size() - nOffset;
    if(nSize <= 0) {
        std::cout << "[FAIL] generateRandomMutations failed size " << nSize << "\n";
        return;
    }
    char buf[1];
    for(int i = 0; i < nRandomCicles; i++){
        int x = rand() % nSize;
        unsigned char ch = data.data()[x + nOffset];
        buf[0] = ch + rand();
        // std::cout << (int)ch << " -> " << (int)buf[0] << "\n";
        data.data()[x + nOffset] = buf[0];
        // data = data.replace(x + nOffset, 1, buf);
    }
    importFromByteArray(data);
    */
}

// ----------------------------------------------------------------

void BNA::appendRandomData(int nRandomCicles) {
    WsjcppLog::err(TAG, "TODO toJson");
    /*QByteArray data = exportToByteArray();

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
    importFromByteArray(data);*/
}

// ----------------------------------------------------------------

nlohmann::json BNA::toJson(){
    WsjcppLog::err(TAG, "TODO toJson");
}

// ----------------------------------------------------------------

BNABit BNA::calc(const std::vector<BNABit> &vInputs, int nOutput){
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
    TAG = "BNAMemoryItem";
}

// ----------------------------------------------------------------

const std::vector<BNABit> &BNAMemoryItem::inputToVectorBool(){
    if (m_vInput.size() == 0) {
        WsjcppLog::err(TAG, "TODO inputToVectorBool");
        // BNAConvertArrayToVBool(input, m_vInput, m_nInputBits);
    }
    return m_vInput;
}

// ----------------------------------------------------------------

const std::vector<BNABit> &BNAMemoryItem::outputToVectorBool(){
    if (m_vOutput.size() == 0) {
        WsjcppLog::err(TAG, "TODO outputToVectorBool");
        // BNAConvertArrayToVBool(output, m_vOutput, m_nOutputBits);
    }
    return m_vOutput;
}

// ----------------------------------------------------------------

BNAMemory::BNAMemory(int nInputBits, int nOutputBits){
    m_nInputBits = nInputBits;
    m_nOutputBits = nOutputBits;
    TAG = "BNAMemory";
}

// ----------------------------------------------------------------

void BNAMemory::load(std::string filename){
    WsjcppLog::err(TAG, "TODO load");
   /* QFile file(filename);
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
    */
};

// ----------------------------------------------------------------

void BNAMemory::save(std::string filename) {
    WsjcppLog::err(TAG, "TODO save");
    /*QFile file(filename);
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
    file.close();*/
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
        WsjcppLog::err(TAG, "TODO printData");
        // std::cerr << m_vItems[i]->input.toHex().toStdString() << " => " << m_vItems[i]->output.toHex().toStdString() << "\n";
    }
}

// ----------------------------------------------------------------

// void BNAMemory::generateData(int nCount){
//     m_vItems.clear();
//     for(int i = 0; i < nCount; i++){
//         BNAMemoryItem memoryItem;
//         memoryItem.input.append(generateRandomString());
//         memoryItem.output = QCryptographicHash::hash(memoryItem.input, QCryptographicHash::Md5);
//         m_vItems.push_back(memoryItem);
//     }
// }

// // ----------------------------------------------------------------

// std::string BNAMemory::alphabet() {
//     return "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890[]{}:,.<>/?\"'\\*&^%$#!-+=";
// }

// // ----------------------------------------------------------------

// std::string BNAMemory::generateRandomString(){
//     std::string sAlphabet = alphabet();
//     int len = qrand() % (m_nInputBits) + 2;
//     std::string str = "";
//     for (int i = 0; i < len; i++) {
//         str += sAlphabet[qrand() % sAlphabet.length()];
//     }
//     return str;
// }

// ----------------------------------------------------------------

BNAProject::BNAProject(){
    m_nInputBits = 1;
    m_nOutputBits = 1;
    m_nDefaultCountNodes = 5;
    TAG = "BNAProject";
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

bool BNAProject::open(std::string sDirPath){
    if (!WsjcppCore::dirExists(sDirPath)) {
        std::cerr << "[ERROR] BNA Project " << sDirPath << " does not exists in this folder.\n";
        return false;
    }
    m_sDirPath = sDirPath;
    loadProjFile();
    m_pBNAMemory = new BNAMemory(m_nInputBits, m_nOutputBits);
    m_pBNAMemory->load(m_sDirPath + "/" + m_sMemoryFileName);

    for (int bitid = 0; bitid < m_nOutputBits; bitid++) {
        std::string m_sBitid = prepareName(bitid);
        std::string subdir = prepareSubdir(bitid);
        std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".bna";
        BNA *pBNA = new BNA();
        pBNA->load(m_sFilename);
        m_mBNA[bitid] = pBNA;
        m_mapResults[bitid] = loadResult(bitid);
    }

    return true;
}

// ----------------------------------------------------------------

std::string BNAProject::prepareName(int nBitId) {
    std::string sName = std::to_string(nBitId);
    while (sName.length() < 3) {
        sName = "0" + sName;
    }
    return sName;
}

// ----------------------------------------------------------------

std::string BNAProject::prepareSubdir(int nBitId){
    std::string m_sBitid = prepareName(nBitId);
    std::string sRet;
    sRet += m_sBitid[0];
    sRet += "/";
    sRet += m_sBitid[1];
    sRet += "/";
    sRet += m_sBitid[2];
    return sRet;
}

// ----------------------------------------------------------------

bool BNAProject::create(std::string sDirPath){
    WsjcppLog::err(TAG, "TODO create");
    /*QDir dir(sDirPath);
    if(dir.exists()){
        std::cerr << "[ERROR] BNA Project " << sDirPath.toStdString() << " already exists.\n";
        return false;
    }
    QDir curr(".");
    curr.mkpath(sDirPath);
    m_sDirPath = sDirPath;

    for (int bitid = 0; bitid < m_nOutputBits; bitid++) {
        std::string m_sBitid = prepareName(bitid);
        std::string subdir = prepareSubdir(bitid);
        curr.mkpath(m_sDirPath + "/" + subdir);
        std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".bna";

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
*/
    return true;
}

// ----------------------------------------------------------------

void BNAProject::saveProjFile(){
    WsjcppLog::err(TAG, "TODO loadProjFile");
    /*QJsonObject proj;
    proj["input_bits"] = m_nInputBits;
    proj["output_bits"] = m_nOutputBits;
    proj["default_count_nodes"] = m_nDefaultCountNodes;
    proj["memory"] = m_sMemoryFileName;

    QJsonDocument doc(proj);
    std::string sProj = doc.toJson(QJsonDocument::Indented);
    std::string sFilename = m_sDirPath + "/bnaproject.json";
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
    file.close();*/
}

// ----------------------------------------------------------------

void BNAProject::loadProjFile(){
    WsjcppLog::err(TAG, "TODO loadProjFile");
    /*std::string sFilename = m_sDirPath + "/bnaproject.json";
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
    std::string sProj = stream.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(sProj.toUtf8());
    QJsonObject proj = doc.object();
    m_nInputBits = proj["input_bits"].toInt();
    m_nOutputBits = proj["output_bits"].toInt();
    m_nDefaultCountNodes = proj["default_count_nodes"].toInt();
    m_sMemoryFileName = proj["memory"].toString();*/
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

int BNAProject::calculate(int nBitId, bool bEnableSleep){

    if(nBitId > m_nOutputBits){
        std::cerr << "Invalid nBitId max possible" << (m_nOutputBits-1) << "\n";
        return 0;
    }
    std::map<int,BNA *>::iterator it;
    it = m_mBNA.find(nBitId);
    if (it == m_mBNA.end()) {
        std::cerr << "Not found nBitId in array\n";
        return 0;
    }

    int nResult = 0;
    BNA *pBNA = m_mBNA[nBitId];
    int nMemorySize = m_pBNAMemory->size();
    for(int i = 0; i < nMemorySize; i++){
        BNAMemoryItem *pBNAMemoryItem = m_pBNAMemory->at(i);
        BNABit bResult = pBNA->calc(pBNAMemoryItem->inputToVectorBool(), 0);
        if (pBNAMemoryItem->outputToVectorBool()[nBitId] == bResult) {
            nResult++;
        }

        if (bEnableSleep && i > 0 && i % 1000 == 0) {
            WsjcppLog::err(TAG, "TODO sleep here");
            // QThread::sleep(1);
        }
    }
    return nResult;
}

// ----------------------------------------------------------------

void BNAProject::saveResult(int bitid, int nResult){
    WsjcppLog::err(TAG, "TODO");

	/*std::string m_sBitid = prepareName(bitid);
	std::string subdir = prepareSubdir(bitid);
	std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".result";
	
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
	file.close();*/
}

// ----------------------------------------------------------------

int BNAProject::loadResult(int bitid){
	/*std::string m_sBitid = prepareName(bitid);
	std::string subdir = prepareSubdir(bitid);
	std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".result";
	
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
    return nResult;*/
}
        
// ----------------------------------------------------------------

std::map<int,int> &BNAProject::getResults(){
    return m_mapResults;
}

// ----------------------------------------------------------------

BNA *BNAProject::getBNA(int nBitId){
    if(nBitId > m_nOutputBits){
        std::cerr << "Invalid bitid max possible" << (m_nOutputBits-1) << "\n";
        return NULL;
    }

    std::map<int, BNA *>::iterator it;
    it = m_mBNA.find(nBitId); 
    if (it == m_mBNA.end()) {
        std::cerr << "Not found nBitId in array\n";
        return NULL;
    }

    return m_mBNA[nBitId];
}

// ----------------------------------------------------------------

void BNAProject::saveBNA(int nBitId){
    if(nBitId > m_nOutputBits){
        std::cerr << "Invalid nBitId max possible" << (m_nOutputBits-1) << "\n";
        return;
    }
    std::map<int, BNA *>::iterator it;
    it = m_mBNA.find(nBitId); 
    if(it == m_mBNA.end()){
        std::cerr << "Not found nBitId in array\n";
        return;
    }

    std::string m_sBitid = prepareName(nBitId);
    std::string subdir = prepareSubdir(nBitId);
    std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".bna";
    m_mBNA[nBitId]->save(m_sFilename);
}

// ----------------------------------------------------------------