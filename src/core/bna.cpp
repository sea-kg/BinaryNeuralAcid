#include <bna.h>
#include <iostream>
#include <iostream>
#include <cstring>
#include <wsjcpp_core.h>

// -----------------------------------------------------------------
// function for convert hex string to array bna bit

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
// BNAStatCalcResults

BNAStatCalcResults::BNAStatCalcResults(int nOutputSize) {
    TAG = "BNAStatCalcResults";
    m_nOutputSize = nOutputSize;
    // init counters
    m_vPrevCounters.resize(m_nOutputSize);
    m_vPrevCountersPercents.resize(m_nOutputSize);
    m_vCurrentCounters.resize(m_nOutputSize);
    m_vCurrentCountersPercents.resize(m_nOutputSize);
    for (int i = 0; i < m_nOutputSize; i++) {
        m_vPrevCounters[i] = 0;
        m_vCurrentCounters[i] = 0;
        m_vCurrentCountersPercents[i] = 0;
        m_vPrevCountersPercents[i] = 0;
    }
}

const std::vector<int> &BNAStatCalcResults::getPrevCounters() const {
    return m_vPrevCounters;
}

const std::vector<int> &BNAStatCalcResults::getPrevCountersPercents() const {
    return m_vPrevCountersPercents;
}

int BNAStatCalcResults::getAllPrevCountersPercents() const {
    return m_nAllPrevCountersPercents;
}

void BNAStatCalcResults::setPrevCounters(const std::vector<int> &vValues) {
    if (m_nOutputSize != vValues.size()) {
        WsjcppLog::throw_err(TAG, "Wrong size");
    }
    m_vPrevCounters = vValues;
}

const std::vector<int> &BNAStatCalcResults::getCurrentCounters() const {
    return m_vCurrentCounters;
}

const std::vector<int> &BNAStatCalcResults::getCurrentCountersPercents() const {
    return m_vCurrentCountersPercents;
}

int BNAStatCalcResults::getAllCurrentCountersPercents() const {
    return m_nAllCurrentCountersPercents;
}

void BNAStatCalcResults::setCurrentCounters(const std::vector<int> &vValues) {
    if (m_nOutputSize != vValues.size()) {
        WsjcppLog::throw_err(TAG, "Wrong size");
    }
    m_vCurrentCounters = vValues;
}

void BNAStatCalcResults::resetCurrentCounters() {
    for (int i = 0; i < m_nOutputSize; i++) {
        m_vCurrentCounters[i] = 0;
    }
}

void BNAStatCalcResults::incrementCurrentCounter(int nIndex) {
    m_vCurrentCounters[nIndex] += 1;
}

void BNAStatCalcResults::calcPercents(int nDataTestsSize) {
    m_nDataTestsSize = nDataTestsSize;
    m_nSummaryDiff = 0;
    int nAllPrev = 0;
    int nAllCurrent = 0;
    for (int i = 0; i < m_nOutputSize; i++) {
        nAllPrev += m_vPrevCounters[i];
        nAllCurrent += m_vCurrentCounters[i];
        int nDiff = m_vCurrentCounters[i] - m_vPrevCounters[i];
        m_nSummaryDiff += nDiff;
        
        m_vPrevCountersPercents[i] = (m_vPrevCounters[i]*100) / m_nDataTestsSize;
        m_vCurrentCountersPercents[i] = (m_vCurrentCounters[i]*100) / m_nDataTestsSize;
        
        // std::cout << "output bit" << i << ": " << m_vPrevCounters[i]  << " (" << m_vPrevCountersPercents[i] << ")% -> "
        //     << m_vCurrentCounters[i] << " (" << m_vCurrentCountersPercents[i] <<  "%) " << " diff: " << nDiff << std::endl;
    }
    int nAllTests = m_nOutputSize * nDataTestsSize;
    m_nAllPrevCountersPercents = (nAllPrev*100) / nAllTests;
    m_nAllCurrentCountersPercents = (nAllCurrent*100) / nAllTests;

    // std::cout << "summary diff: " << m_nSummaryDiff << std::endl;
}

int BNAStatCalcResults::getSummaryDiff() {
    return m_nSummaryDiff;
}

// -----------------------------------------------------------------
// BNAModificationModel

BNAModificationModel::BNAModificationModel() {
    this->reset();
}

BNAModificationModel::BNAModificationModel(int nMutationCicles, int nAddCicles, int nRemoveCicles) {
    this->reset();
    m_nMutationCicles = nMutationCicles;
    m_nAddCicles = nAddCicles;
    m_nRemoveCicles = nRemoveCicles;
}

void BNAModificationModel::update(int nMutationCicles, int nAddCicles, int nRemoveCicles) {
    this->reset();
    m_nMutationCicles = nMutationCicles;
    m_nAddCicles = nAddCicles;
    m_nRemoveCicles = nRemoveCicles;
}

void BNAModificationModel::reset() {
    m_nMutationCicles = 0;
    m_nAddCicles = 0;
    m_nRemoveCicles = 0;
}

int BNAModificationModel::getMutationCicles() const {
    return m_nMutationCicles;
}

int BNAModificationModel::getAddCicles() const {
    return m_nAddCicles;
}

int BNAModificationModel::getRemoveCicles() const {
    return m_nRemoveCicles;
}

void BNAModificationModel::print() const {
    std::cout << m_nMutationCicles << ";" << m_nAddCicles << ";" << m_nRemoveCicles << ";" << std::endl;
}

// -----------------------------------------------------------------
// BNA

BNA::BNA() {
    m_vNodesInput.push_back(new BNANodeInput(0));
    m_vNodesOutput.push_back(new BNANodeOutput(0, 0));
    registryOperationType(new BNAOperXor());
    registryOperationType(new BNAOperNotXor());
    registryOperationType(new BNAOperAnd());
    registryOperationType(new BNAOperOr());
    m_nOperSize = m_vOperationList.size();
    TAG = "BNA";
    m_nBnaVersion = 4;
    m_nBnaRevision = 0;
}

BNA::BNA(int nInputSize, int nOutputSize) : BNA() {
    for (int i = 0; i < nInputSize; i++) {
        m_vNodesInput.push_back(new BNANodeInput(i));
    }
    for (int i = 0; i < nOutputSize; i++) {
        m_vNodesOutput.push_back(new BNANodeOutput(i, i));
    }
    m_bCompiled = false;
}

unsigned int BNA::getInputSize() {
    return m_vNodesInput.size();
}

unsigned int BNA::getNodesSize() {
    return m_vNodes.size();
}

unsigned int BNA::getOutputSize() {
    return m_vNodesOutput.size();
}

BNA::~BNA() {
    m_vOperations.clear();
    m_vOperationList.clear();
    m_vNodes.clear();
    clearResources();
}

// ----------------------------------------------------------------

bool BNA::load(const std::string &sFilename){
    clearResources();

    std::string sFilename0 = sFilename + ".bna";
    if (!WsjcppCore::fileExists(sFilename0)) {
        WsjcppLog::err(TAG, "load: file not exists '" + sFilename0 + "'");
        return false;
    }
    std::ifstream file;
    file.open(sFilename0, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        WsjcppLog::err(TAG, "load: could not open file to read '" + sFilename0 + "'");
        return false;
    }
    bool bResult = readFromFileBna(file);
    file.close(); // TODO file will be automaticly closed on return of scope?
    return bResult;
}

// ----------------------------------------------------------------

bool BNA::save(const std::string &sFilename){
    std::string sFilename0 = sFilename + ".bna";
    if (WsjcppCore::fileExists(sFilename0)) {
        if (!WsjcppCore::removeFile(sFilename0)) {
            WsjcppLog::err(TAG, "save: could not remove file '" + sFilename0 + "'");
            return false;
        }
    }
    std::ofstream file;
    file.open(sFilename0, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        WsjcppLog::err(TAG, "save: could not open file to write: " + sFilename0);
        return false;
    }
    // update ids

    for (int i = 0; i < m_vNodes.size(); i++) {
        int nIndex = i + m_vNodesInput.size();
        m_vNodes[i]->setId(nIndex);
    }
    m_nBnaRevision++;
    bool bResult = writeToFileBna(file);
    file.close(); // TODO file will be automaticly closed on return of scope?
    return bResult;
}

// ----------------------------------------------------------------

void BNA::randomGenerate(int nInputSize, int nOutputSize, int nSize){
    clearResources();
    m_nBnaRevision = 0;
    for (int i = 0; i < nInputSize; i++) {
        m_vNodesInput.push_back(new BNANodeInput(i));
    }
    for (int i = 0; i < nSize; i++) {
        BNANode *pItem = new BNANode();
        pItem->setX(rand());
        pItem->setY(rand());
        int nOper = rand() % m_nOperSize;
        pItem->setOperationType(m_vOperationList[nOper]->type());
        m_vNodes.push_back(pItem);
    }
    for (int i = 0; i < nOutputSize; i++) {
        m_vNodesOutput.push_back(new BNANodeOutput(i, rand()));
    }
    compile();
}

int BNA::addNode(int nInX, int nInY, const std::string &sOperType) {
    BNANode *pItem = new BNANode();
    pItem->setX(nInX);
    pItem->setY(nInY);
    pItem->setOperationType(sOperType);
    m_vNodes.push_back(pItem);
    m_bCompiled = false;
    return m_vNodes.size() - 1 + m_vNodesInput.size();
}

void BNA::clearResources() {
    clearCalcExprsVars();
    // clear input nodes
    for(int i = 0; i < m_vNodesInput.size(); i++){
        delete m_vNodesInput[i];
    }
    m_vNodesInput.clear();

    // clear nodes
    for(int i = 0; i < m_vNodes.size(); i++){
        delete m_vNodes[i];
    }
    m_vNodes.clear();

    // clear output nodes
    for(int i = 0; i < m_vNodesOutput.size(); i++){
        delete m_vNodesOutput[i];
    }
    m_vNodesOutput.clear();
}

bool BNA::compile() {
    if (m_bCompiled) {
        return true; // already compiled
    }
    
    // std::cout << "Compiling..." << std::endl;

    clearCalcExprsVars();

    // prepare input nodes
    for (unsigned int i  = 0; i < m_vNodesInput.size(); i++) {
        BNAVar<BNABit> *pVar = new BNAVar<BNABit>();
        pVar->setValue(B_0);
        pVar->setName("in" + std::to_string(i));
        m_vCalcInputVars.push_back(pVar);
    }

    normalizeNodes();

    int nItemsSize = m_vNodes.size();
    for (int i = 0; i < m_vNodes.size(); i++) {
        int x = m_vNodes[i]->getX();
        int y = m_vNodes[i]->getY();
        std::string sOperationType = m_vNodes[i]->getOperationType();
        BNAExpression<BNABit> *pExpr = new BNAExpression<BNABit>();
        pExpr->setOperandLeft(getVarByIndex(x));
        pExpr->setOperandRight(getVarByIndex(y));
        pExpr->oper(m_vOperations[sOperationType]);
        BNAVar<BNABit> *pVar = new BNAVar<BNABit>();
        pVar->setName("node" + std::to_string(i));
        m_vCalcVars.push_back(pVar);
        pExpr->out(pVar);
        m_vCalcExprs.push_back(pExpr);
        if (nItemsSize - i <= (int)m_vNodesOutput.size()) {
            m_vCalcOutVars.push_back(pVar);
        }
    }

    for (int i = 0; i < m_vNodesOutput.size(); i++) {
        int nIndex = m_vNodesOutput[i]->getInputNodeIndex();
        m_vCalcOutVars.push_back(getVarByIndex(nIndex));
    }

    // std::cout << "m_vCalcOutVars.size() = " << m_vCalcOutVars.size() << std::endl;
    // std::cout << "m_vNodes.size() = " << m_vNodes.size() << std::endl;
    // std::cout << "m_vCalcExprs.size() = " << m_vCalcExprs.size() << std::endl;

    m_bCompiled = true;
    // std::cout << "Compiled!" << std::endl;
    return true;
}

// ----------------------------------------------------------------

void BNA::compare(BNA &bna){
    if(bna.getInputSize() != m_vNodesInput.size()){
        std::cout << "inputs not equals\n";
    }
    if(bna.getOutputSize() != m_vNodesOutput.size()){
        std::cout << "outputs not equals\n";
    }
    if(bna.m_vNodes.size() == m_vNodes.size()){
        for (int i = 0; i < m_vNodes.size(); i++) {
            if (m_vNodes[i]->getOperationType() != bna.m_vNodes[i]->getOperationType()){
                std::cout << "\t T not equal in " << i << "\n";
            }

            if (m_vNodes[i]->getX() != bna.m_vNodes[i]->getX()) {
                std::cout << "\t X not equal in " << i << "\n";
            }

            if (m_vNodes[i]->getY() != bna.m_vNodes[i]->getY()) {
                std::cout << "\t Y not equal in " << i << "\n";
            }
        }
    } else {
        std::cout << "Item size not equals " << bna.m_vNodes.size() << " != " << m_vNodes.size() << " \n";
    }

    if(bna.m_vCalcExprs.size() == m_vCalcExprs.size()){
        for(int i = 0; i < m_vCalcExprs.size(); i++){
            if(m_vCalcExprs[i]->getOperandLeft()->getName() != bna.m_vCalcExprs[i]->getOperandLeft()->getName()){
                std::cout << "\t operand_left not equal in " << i << "\n";
            }
            if(m_vCalcExprs[i]->op2()->getName() != bna.m_vCalcExprs[i]->op2()->getName()){
                std::cout << "\t operand_right not equal in " << i << "\n";
            }
            if(m_vCalcExprs[i]->out()->getName() != bna.m_vCalcExprs[i]->out()->getName()){
                std::cout << "\t out not equal in " << i << "\n";
            }
            if(m_vCalcExprs[i]->oper()->type() != bna.m_vCalcExprs[i]->oper()->type()){
                std::cout << "\t oper not equal in " << i << "\n";
            }
        }
    } else {
        std::cout << "Exprs size not equals " << bna.m_vNodes.size() << " != " << m_vNodes.size() << " \n";
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
    for(unsigned int i = 0; i < m_vNodesInput.size(); i++){
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
    
    
    int nodes = m_vNodesInput.size();
    for(int i = 0; i < m_vNodes.size(); i++){
        std::string sX = (m_vNodes[i]->getX() < m_vNodesInput.size() ? "in" : "node") + std::to_string(m_vNodes[i]->getX());
        std::string sY = (m_vNodes[i]->getY() < m_vNodesInput.size() ? "in" : "node") + std::to_string(m_vNodes[i]->getY());
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

const std::vector<BNANodeInput *> &BNA::getNodesInput() {
    return m_vNodesInput;
}

const std::vector<BNANode *> &BNA::getNodes() {
    return m_vNodes;
}

const std::vector<BNANodeOutput *> &BNA::getNodesOutput() {
    return m_vNodesOutput;
}

int BNA::calculateDepth(int n) {
    int nNodesInputSize = m_vNodesInput.size();
    if (n < nNodesInputSize) {
        return 1;
    }
    n = n - nNodesInputSize;
    int nNodesSize = m_vNodes.size();
    if (n < nNodesSize) {
        BNANode *pNode = m_vNodes[n];
        int nLeft = this->calculateDepth(pNode->getX());
        int nRight = this->calculateDepth(pNode->getY());
        return std::max(nLeft, nRight) + 1;
    }
    n = n - nNodesSize;
    int nNodesOutputSize = m_vNodesOutput.size();
    if (n < nNodesOutputSize) {
        BNANodeOutput *pNode = m_vNodesOutput[n];
        return this->calculateDepth(pNode->getInputNodeIndex()) + 1;
    }
    return -1000;
}

bool BNA::readFromFileBna(std::ifstream &file){
    clearResources();
    std::string sStr; 
    file >> sStr;
    if (sStr != "BNA") {
        WsjcppLog::err(TAG, "readFromFileBna, is not a BNA file");
        return false;
    }
    int nBnaVersion = readParam(file, "version");
    if (nBnaVersion != m_nBnaVersion) {
        WsjcppLog::err(TAG, "readFromFileBna, Version expected '" + std::to_string(m_nBnaVersion) + "', but got '" + std::to_string(nBnaVersion) + "'");
        return false;
    }
    m_nBnaRevision = readParam(file, "revision");
    int nInputSize = readParam(file, "input");
    int nNodesSize = readParam(file, "nodes");
    int nOutputSize = readParam(file, "output");
    
    for (int i = 0; i < nInputSize; i++) {
        m_vNodesInput.push_back(new BNANodeInput(i));
    }
    
    // read nodes
    for (int i = 0; i < nNodesSize; i++) {
        BNANode *pItem = new BNANode();
        pItem->readFromFile(file);
        m_vNodes.push_back(pItem);
    }

    // read outputs
    for (int i = 0; i < nOutputSize; i++) {
        m_vNodesOutput.push_back(new BNANodeOutput(i, i));
        int nInputNodeIndex = readParam(file, "output");
        m_vNodesOutput[i]->setInputNodeIndex(nInputNodeIndex);
    }
    
    return compile(); // need for process expressions
}

int BNA::readParam(std::ifstream &file, const std::string &sParamName) {
    std::string sKeyword = "";
    file >> sKeyword;
    if (sKeyword != sParamName) {
        WsjcppLog::throw_err(TAG, "readParam, Expected keyword '" + sParamName + "'");
        return -1;
    }
    int nRet;
    file >> nRet;
    return nRet;
}

bool BNA::writeToFileBna(std::ofstream &file){
    // basic information about file
    file << "BNA version " << m_nBnaVersion << "\n";
    file << "revision " << m_nBnaRevision << "\n";
    file << "input " << m_vNodesInput.size() << "\n";
    file << "nodes " << m_vNodes.size() << "\n";
    file << "output " << m_vNodesOutput.size() << "\n";
    for (int i = 0; i < m_vNodes.size(); i++) {
        m_vNodes[i]->writeToFile(file);
    }
    for (int i = 0; i < m_vNodesOutput.size(); i++) {
        m_vNodesOutput[i]->writeToFile(file);
    }
    return true;
}

bool BNA::registryOperationType(IBNAOper<BNABit> *pOper) {
    // TODO check aready registered
    m_vOperations[pOper->type()] = pOper;
    m_vOperationList.push_back(pOper);
    return true;
}

void BNA::randomModify(const BNAModificationModel *pModel) {
    for (int i = 0; i < pModel->getRemoveCicles(); i++) {
        if (m_vNodes.size() == 0) {
            break;
        }
        int nIndex = rand() % m_vNodes.size();
        BNANode *pItem = m_vNodes[nIndex];
        m_vNodes.erase(m_vNodes.begin() + nIndex);
        delete pItem;
    }
    for (int i = 0; i < pModel->getMutationCicles(); i++) {
        int nItemIndex = rand() % (m_vNodes.size() + m_vNodesOutput.size());
        if (nItemIndex < m_vNodes.size()) {
            m_vNodes[nItemIndex]->setX(rand());
            m_vNodes[nItemIndex]->setY(rand());
            int nOper = rand() % m_nOperSize;
            m_vNodes[nItemIndex]->setOperationType(m_vOperationList[nOper]->type());
        } else {
            nItemIndex = nItemIndex - m_vNodes.size();
            m_vNodesOutput[nItemIndex]->setInputNodeIndex(rand());
        }
    }
    for (int i = 0; i < pModel->getAddCicles(); i++) {
        BNANode *pItem = new BNANode();
        pItem->setX(rand());
        pItem->setY(rand());
        int nOper = rand() % m_nOperSize;
        pItem->setOperationType(m_vOperationList[nOper]->type());
        m_vNodes.push_back(pItem);
    }
    if (pModel->getMutationCicles() > 0 || pModel->getAddCicles() > 0 || pModel->getRemoveCicles() > 0) {
        m_bCompiled = false;
        normalizeNodes();
    }
}

nlohmann::json BNA::toJson(){
    WsjcppLog::err(TAG, "TODO toJson");
}

BNABit BNA::calc(const std::vector<BNABit> &vInputs, int nOutput){
    // prepare calculate exprs
    if (!m_bCompiled) {
        // std::cout << "Not compiled" << std::endl;
        compile();
    }

    if ((unsigned int)vInputs.size() != m_vNodesInput.size()) {
        std::cerr << "[ERROR] invalid input count " << vInputs.size() << "(Expected: " << m_vNodesInput.size() << ") \n";
        return B_0;
    }

    for (unsigned int i  = 0; i < m_vNodesInput.size(); i++) {
        m_vCalcInputVars[i]->setValue(vInputs[i]);
    }

    for (int i = 0; i < m_vCalcExprs.size(); i++) {
        m_vCalcExprs[i]->exec();
    }

    return m_vCalcOutVars[nOutput]->getValue();
}

void BNA::clearCalcExprsVars() {
    
    // clear input vars
    for (int i = 0; i < m_vCalcInputVars.size(); i++) {
        delete m_vCalcInputVars[i];
    }
    m_vCalcInputVars.clear();

    // clear expressions
    for (int i = 0; i < m_vCalcExprs.size(); i++) {
        delete m_vCalcExprs[i];
    }
    m_vCalcExprs.clear();

    // clear vars
    for (int i = 0; i < m_vCalcVars.size(); i++) {
        delete m_vCalcVars[i];
    }
    m_vCalcVars.clear();

    // clear output vars
    m_vCalcOutVars.clear();
    m_bCompiled = false;
}

void BNA::normalizeNodes() {
    int nNodesSize = m_vNodesInput.size();
    // normalize nodes
    for (int i = 0; i < m_vNodes.size(); i++) {
        m_vNodes[i]->setX(m_vNodes[i]->getX() % nNodesSize);
        m_vNodes[i]->setY(m_vNodes[i]->getY() % nNodesSize);
        nNodesSize++;
    }
    // normalize nodes output
    for (int i = 0; i < m_vNodesOutput.size(); i++) {
        m_vNodesOutput[i]->setInputNodeIndex( m_vNodesOutput[i]->getInputNodeIndex() % nNodesSize);
    }
    // TODO remove nodes to noway
    std::vector<int> vToRemoving;
    for (int i = 0; i < m_vNodes.size(); i++) {
        int nNodeIndex = i + m_vNodesInput.size();
        int nLinks = 0;
        for (int i0 = 0; i0 < m_vNodes.size(); i0++) {
            if (m_vNodes[i0]->getX() == nNodeIndex || m_vNodes[i0]->getY() == nNodeIndex) {
                nLinks++;
            }
        }
        for (int i0 = 0; i0 < m_vNodesOutput.size(); i0++) {
            if (m_vNodesOutput[i0]->getInputNodeIndex() == nNodeIndex) {
                nLinks++;
            }
        }
        if (nLinks == 0) {
            vToRemoving.push_back(nNodeIndex);
        }
    }
    // std::cout << "TODO removing all size: " << vToRemoving.size() << std::endl;
    for (int i = vToRemoving.size() - 1; i >= 0; i--) {
        int nNodeIndex = vToRemoving[i];
        int nArrayIndex = vToRemoving[i] - m_vNodesInput.size();
        // std::cout << "Will be removed [" << nNodeIndex << "] in array = " << nArrayIndex << std::endl;
        if (nNodeIndex > m_vNodes.size() + m_vNodesInput.size()) {
            WsjcppLog::throw_err(TAG, "Node Index very big much");
        }
    }

    for (int i = vToRemoving.size() - 1; i >= 0; i--) {
        int nNodeIndex = vToRemoving[i];
        int nArrayIndex = vToRemoving[i] - m_vNodesInput.size();
        // std::cout << "TODO removing nNodeIndex = " << nNodeIndex << " in array = " << nArrayIndex << std::endl;
        // std::cout << "TODO removing m_vNodes.size() = " << m_vNodes.size() << std::endl;
        if (nArrayIndex < 0) {
            continue;
        }

        m_vNodes.erase(m_vNodes.begin() + nArrayIndex);
        for (int x = 0; x < m_vNodes.size(); x++) {
            if (m_vNodes[x]->getX() >= nNodeIndex) {
                m_vNodes[x]->setX(m_vNodes[x]->getX() - 1);
            }
            if (m_vNodes[x]->getY() >= nNodeIndex) {
                m_vNodes[x]->setY(m_vNodes[x]->getY() - 1);
            }
        }
        
        for (int x = 0; x < m_vNodesOutput.size(); x++) {
            if (m_vNodesOutput[x]->getInputNodeIndex() >= nNodeIndex) {
                m_vNodesOutput[x]->setInputNodeIndex(m_vNodesOutput[x]->getInputNodeIndex() - 1);
            }
        }
    }
}

BNAVar<BNABit> *BNA::getVarByIndex(int nIndex) {
    if (nIndex < m_vCalcInputVars.size()) {
        return m_vCalcInputVars[nIndex];
    }
    nIndex = nIndex - m_vCalcInputVars.size();
    if (nIndex < m_vCalcVars.size()) {
        return m_vCalcVars[nIndex];
    }
    // std::cout << "nIndex = " << nIndex << std::endl;
    // std::cout << "m_vCalcVars.size() = " << m_vCalcVars.size() << std::endl;
    WsjcppLog::throw_err(TAG, "out of rande index of var");
    nIndex = nIndex - m_vCalcVars.size();
    return m_vCalcOutVars[nIndex];
}

// ----------------------------------------------------------------
// BNAMemoryItem

BNAMemoryItem::BNAMemoryItem(int nInputBits, int nOutputBits) {
    m_nInputBits = nInputBits;
    m_nOutputBits = nOutputBits;
    TAG = "BNAMemoryItem";
}

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

    m_vNodes.clear();

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
        m_vNodes.push_back(pBNAMemoryItem);
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
    stream << m_nInputBits << m_nOutputBits << m_vNodes.size();
    for (int i = 0; i < m_vNodes.size(); i++) {
        stream << m_vNodes[i]->input << m_vNodes[i]->output;
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
        BNABit bResult; // = pBNA->calc(pBNAMemoryItem->inputToVectorBool(), 0);
        // if (pBNAMemoryItem->outputToVectorBool()[nBitId] == bResult) {
        //     nResult++;
        // }

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