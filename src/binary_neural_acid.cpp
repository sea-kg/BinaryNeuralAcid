#include "binary_neural_acid.h"
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

// -----------------------------------------------------------------
// BinaryNeuralAcidBit4

BinaryNeuralAcidBit4::BinaryNeuralAcidBit4(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2, BinaryNeuralAcidBit b3, BinaryNeuralAcidBit b4) : b1(b1), b2(b2), b3(b3), b4(b4) {

}

void BinaryNeuralAcidBit4::appendToVector(std::vector<BinaryNeuralAcidBit> &vars){
    vars.push_back(b1);
    vars.push_back(b2);
    vars.push_back(b3);
    vars.push_back(b4);
}

// -----------------------------------------------------------------
// BinaryNeuralAcidBitConvertor

std::string BinaryNeuralAcidBitConvertor::toBinString(const BinaryNeuralAcidBit& bit) {
    if (bit == B_0) {
        return std::string("0");
    } else if (bit == B_1) {
        return std::string("1");
    }
    return std::string("?");
}

std::string BinaryNeuralAcidBitConvertor::toBinString(const BinaryNeuralAcidBit4& bit4) {
    std::string sResult;
    if (bit4.b1 == B_0) sResult += "0"; else if (bit4.b1 == B_1) sResult += "1"; else sResult += "?";
    if (bit4.b2 == B_0) sResult += "0"; else if (bit4.b2 == B_1) sResult += "1"; else sResult += "?";
    if (bit4.b3 == B_0) sResult += "0"; else if (bit4.b3 == B_1) sResult += "1"; else sResult += "?";
    if (bit4.b4 == B_0) sResult += "0"; else if (bit4.b4 == B_1) sResult += "1"; else sResult += "?";
    return sResult;
}

std::string BinaryNeuralAcidBitConvertor::toBinStringFromHexString(const std::string& sHex) {
    const std::string sAlphabetLower = "0123456789abcdef";
    const std::string sAlphabetUpper = "0123456789ABCDEF";

    // initMapHEX2BIN();
    std::string sResult = "";
    for (int i = 0; i < sHex.length(); i++){
        char ch = sHex.at(i);
        size_t index = sAlphabetLower.find(ch);
        if (index == -1) {
            index = sAlphabetUpper.find(ch);
        }

        if (index == -1) {
            sResult += "????";
        } else {
            sResult += ((index >> 3) & 0x01) == 0x01 ? "1" : "0";
            sResult += ((index >> 2) & 0x01) == 0x01 ? "1" : "0";
            sResult += ((index >> 1) & 0x01) == 0x01 ? "1" : "0";
            sResult += ((index >> 0) & 0x01) == 0x01 ? "1" : "0";
        }
    }
    return sResult;
}

std::string BinaryNeuralAcidBitConvertor::toBinStringFromBits(const std::vector<BinaryNeuralAcidBit>& bits) {
    std::string sResult;
    for (int i = 0; i < bits.size(); i++) {
        BinaryNeuralAcidBit bit = bits[i];
        if (bit == B_1) {
            sResult += '1';
        } else if (bit == B_0) {
            sResult += '0';
        } else {
            std::cerr << "Skip bit '" << (int)bit << "', because expected 'B_0' or 'B_1'" << std::endl;
        }
    }
    return sResult;
}

std::string BinaryNeuralAcidBitConvertor::toHexString(const BinaryNeuralAcidBit4& bit4) {
    const std::string sAlphabet = "0123456789abcdef";
    short index = 0x00;
    index |= (bit4.b4 << 0) & 0x01;
    index |= (bit4.b3 << 1) & 0x02;
    index |= (bit4.b2 << 2) & 0x04;
    index |= (bit4.b1 << 3) & 0x08;
    if (index < 0 || index > 15) {
        return "?";
    }
    return std::string(1, sAlphabet[index]);
}

std::string BinaryNeuralAcidBitConvertor::toHexStringFromBinString(const std::string& sBin) {
    std::string sResult = "";
    const std::string sAlpabet = "0123456789abcdef";
    unsigned char c = 0;
    int i = 0;
    int sum = 0;
    for (i = 0; i < sBin.length(); i++) {
        char ch = sBin.at(i);
        if (ch != '1' && ch != '0') {
            // skip to next
            i += 4 - (i % 4) - 1; // in next lopp will be +1
            if (i < sBin.length()) {
                sResult += '?';
            }
            c = 0x00;
            continue;
        }
        c = (c << 1) | (ch == '1' ? 0x01 : 0x00);
        if ((i+1) % 4 == 0) {
            sResult += sAlpabet[c];
            c = 0x00;
        }
    }
    i++;
    if (i % 4 == 0 && i < sBin.length()) {
        sResult += sAlpabet[c];
    }
    return sResult;
}

std::string BinaryNeuralAcidBitConvertor::toHexStringFromBits(const std::vector<BinaryNeuralAcidBit>& bits) {
    std::string sBin = BinaryNeuralAcidBitConvertor::toBinStringFromBits(bits);
    return BinaryNeuralAcidBitConvertor::toHexStringFromBinString(sBin);
}

std::vector<BinaryNeuralAcidBit> BinaryNeuralAcidBitConvertor::toBitsFromBinString(const std::string& sBin) {
    std::vector<BinaryNeuralAcidBit> vResult;
    for (int i = 0; i < sBin.length(); i++) {
        char ch = sBin.at(i);
        if (ch == '1') {
            vResult.push_back(B_1);
        } else if (ch == '0') {
            vResult.push_back(B_0);
        } else {
            std::cerr << "Skip character '" << ch << "', because expected '0' or '1'" << std::endl;
        }
    }
    return vResult;
}

std::vector<BinaryNeuralAcidBit> BinaryNeuralAcidBitConvertor::toBitsFromHexString(const std::string& sHex) {
    std::string sBin = BinaryNeuralAcidBitConvertor::toBinStringFromHexString(sHex);
    return BinaryNeuralAcidBitConvertor::toBitsFromBinString(sBin);
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationBitXor

std::string BinaryNeuralAcidOperationBitXor::type(){ return std::string("XOR"); }

BinaryNeuralAcidBit BinaryNeuralAcidOperationBitXor::calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 ^ c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationBitNotXor

std::string BinaryNeuralAcidOperationBitNotXor::type(){ return std::string("NXOR"); }

BinaryNeuralAcidBit BinaryNeuralAcidOperationBitNotXor::calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (!(c1 ^ c2)) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationBitAnd

std::string BinaryNeuralAcidOperationBitAnd::type() {
    return std::string("AND");
}

BinaryNeuralAcidBit BinaryNeuralAcidOperationBitAnd::calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2) {
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 & c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationBitOr

std::string BinaryNeuralAcidOperationBitOr::type(){ return std::string("OR"); }

BinaryNeuralAcidBit BinaryNeuralAcidOperationBitOr::calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 | c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}


// -----------------------------------------------------------------
// BinaryNeuralAcidGraphNode 

BinaryNeuralAcidGraphNode::BinaryNeuralAcidGraphNode(unsigned short x, unsigned short y, const std::string &sOperationType){
    TAG = "BinaryNeuralAcidGraphNode";
    m_nX = x;
    m_nY = y;
    m_nId = 0;
    m_sOperationType = sOperationType;
}

BinaryNeuralAcidGraphNode::BinaryNeuralAcidGraphNode() {
    TAG = "BinaryNeuralAcidGraphNode";
    m_nX = 0;
    m_nY = 0;
    m_nId = 0;
    m_sOperationType = "";
}

unsigned short BinaryNeuralAcidGraphNode::getX(){
    return m_nX;
}

void BinaryNeuralAcidGraphNode::setX(unsigned short nX){
    m_nX = nX;
}

unsigned short BinaryNeuralAcidGraphNode::getY(){
    return m_nY;
}

void BinaryNeuralAcidGraphNode::setY(unsigned short nY){
    m_nY = nY;
}

std::string BinaryNeuralAcidGraphNode::getOperationType(){
    return m_sOperationType;
}

void BinaryNeuralAcidGraphNode::setOperationType(const std::string &sOperationType){
    m_sOperationType = sOperationType;
}

unsigned short BinaryNeuralAcidGraphNode::getId(){
    return m_nId;
}

void BinaryNeuralAcidGraphNode::setId(unsigned short nId){
    m_nId = nId;
}

void BinaryNeuralAcidGraphNode::readFromFile(std::ifstream &file){
    std::string sKeyword = "";
    file >> sKeyword;
    if (sKeyword != "node") {
        throw std::runtime_error("BinaryNeuralAcidGraphNode::readFromFile, Expected keyword 'node'");
    }
    file >> sKeyword;
    if (sKeyword != "id") {
        throw std::runtime_error("BinaryNeuralAcidGraphNode::readFromFile, Expected keyword 'id'");
    }
    file >> m_nId;
    file >> sKeyword;
    if (sKeyword != "input") {
        throw std::runtime_error("BinaryNeuralAcidGraphNode::readFromFile, Expected keyword 'input'");
    }
    file >> m_nX;
    file >> m_nY;
    file >> sKeyword;
    if (sKeyword != "function") {
        throw std::runtime_error("BinaryNeuralAcidGraphNode::readFromFile, Expected keyword 'function'");
    }
    file >> m_sOperationType;
}

void BinaryNeuralAcidGraphNode::writeToFile(std::ofstream &file){
    file << "node id " << m_nId << " input " << m_nX << " " << m_nY << " function " << m_sOperationType << "\n";
}

// -----------------------------------------------------------------
// BinaryNeuralAcidGraphNodeInput

BinaryNeuralAcidGraphNodeInput::BinaryNeuralAcidGraphNodeInput(unsigned short nIndex){
    m_nIndex = nIndex;
}

unsigned short BinaryNeuralAcidGraphNodeInput::getIndex() {
    return m_nIndex;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidGraphNodeOutput

BinaryNeuralAcidGraphNodeOutput::BinaryNeuralAcidGraphNodeOutput(unsigned short nOutputIndex, unsigned short nInputNodeIndex){
    m_nOutputIndex = nOutputIndex;
    m_nInputNodeIndex = nInputNodeIndex;
}

unsigned short BinaryNeuralAcidGraphNodeOutput::getOutputIndex() {
    return m_nOutputIndex;
}

unsigned short BinaryNeuralAcidGraphNodeOutput::getInputNodeIndex() {
    return m_nInputNodeIndex;
}

void BinaryNeuralAcidGraphNodeOutput::setInputNodeIndex(unsigned short nInputNodeIndex) {
    m_nInputNodeIndex = nInputNodeIndex;
}

void BinaryNeuralAcidGraphNodeOutput::writeToFile(std::ofstream &file) {
    file << "output " << m_nInputNodeIndex << "\n";
}

// -----------------------------------------------------------------
// BinaryNeuralAcidStatCalcResults

BinaryNeuralAcidStatCalcResults::BinaryNeuralAcidStatCalcResults(int nOutputSize) {
    TAG = "BinaryNeuralAcidStatCalcResults";
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

const std::vector<int> &BinaryNeuralAcidStatCalcResults::getPrevCounters() const {
    return m_vPrevCounters;
}

const std::vector<int> &BinaryNeuralAcidStatCalcResults::getPrevCountersPercents() const {
    return m_vPrevCountersPercents;
}

int BinaryNeuralAcidStatCalcResults::getAllPrevCountersPercents() const {
    return m_nAllPrevCountersPercents;
}

void BinaryNeuralAcidStatCalcResults::setPrevCounters(const std::vector<int> &vValues) {
    if (m_nOutputSize != vValues.size()) {
        throw std::runtime_error("Wrong size");
    }
    m_vPrevCounters = vValues;
}

const std::vector<int> &BinaryNeuralAcidStatCalcResults::getCurrentCounters() const {
    return m_vCurrentCounters;
}

const std::vector<int> &BinaryNeuralAcidStatCalcResults::getCurrentCountersPercents() const {
    return m_vCurrentCountersPercents;
}

int BinaryNeuralAcidStatCalcResults::getAllCurrentCountersPercents() const {
    return m_nAllCurrentCountersPercents;
}

void BinaryNeuralAcidStatCalcResults::setCurrentCounters(const std::vector<int> &vValues) {
    if (m_nOutputSize != vValues.size()) {
        throw std::runtime_error("Wrong size");
    }
    m_vCurrentCounters = vValues;
}

void BinaryNeuralAcidStatCalcResults::resetCurrentCounters() {
    for (int i = 0; i < m_nOutputSize; i++) {
        m_vCurrentCounters[i] = 0;
    }
}

void BinaryNeuralAcidStatCalcResults::incrementCurrentCounter(int nIndex) {
    m_vCurrentCounters[nIndex] += 1;
}

void BinaryNeuralAcidStatCalcResults::calcPercents(int nDataTestsSize) {
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

int BinaryNeuralAcidStatCalcResults::getSummaryDiff() {
    return m_nSummaryDiff;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidModificationModel

BinaryNeuralAcidModificationModel::BinaryNeuralAcidModificationModel() {
    this->reset();
}

BinaryNeuralAcidModificationModel::BinaryNeuralAcidModificationModel(int nMutationCicles, int nAddCicles, int nRemoveCicles) {
    this->reset();
    m_nMutationCicles = nMutationCicles;
    m_nAddCicles = nAddCicles;
    m_nRemoveCicles = nRemoveCicles;
}

void BinaryNeuralAcidModificationModel::update(int nMutationCicles, int nAddCicles, int nRemoveCicles) {
    this->reset();
    m_nMutationCicles = nMutationCicles;
    m_nAddCicles = nAddCicles;
    m_nRemoveCicles = nRemoveCicles;
}

void BinaryNeuralAcidModificationModel::reset() {
    m_nMutationCicles = 0;
    m_nAddCicles = 0;
    m_nRemoveCicles = 0;
}

int BinaryNeuralAcidModificationModel::getMutationCicles() const {
    return m_nMutationCicles;
}

int BinaryNeuralAcidModificationModel::getAddCicles() const {
    return m_nAddCicles;
}

int BinaryNeuralAcidModificationModel::getRemoveCicles() const {
    return m_nRemoveCicles;
}

void BinaryNeuralAcidModificationModel::print() const {
    std::cout << m_nMutationCicles << ";" << m_nAddCicles << ";" << m_nRemoveCicles << ";" << std::endl;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidConfig

BinaryNeuralAcidConfig::BinaryNeuralAcidConfig() {
    // default
    m_nInputSize = 1;
    m_nOutputSize = 1;
    m_nNodesSize = 10;
}

BinaryNeuralAcidConfig &BinaryNeuralAcidConfig::setInputSize(int nInputSize) {
    m_nInputSize = nInputSize;
    return *this;
}

BinaryNeuralAcidConfig &BinaryNeuralAcidConfig::setNodesSize(int nNodesSize) {
    m_nNodesSize = nNodesSize;
    return *this;
}

BinaryNeuralAcidConfig &BinaryNeuralAcidConfig::setOutputSize(int nOutputSize) {
    m_nOutputSize = nOutputSize;
    return *this;
}

int BinaryNeuralAcidConfig::getInputSize() const {
    return m_nInputSize;
}

int BinaryNeuralAcidConfig::getNodesSize() const {
    return m_nNodesSize;
}

int BinaryNeuralAcidConfig::getOutputSize() const {
    return m_nOutputSize;
}


// -----------------------------------------------------------------
// BinaryNeuralAcid

BinaryNeuralAcid::BinaryNeuralAcid() {
    m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(0));
    m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(0, 0));
    registryOperationType(new BinaryNeuralAcidOperationBitXor());
    registryOperationType(new BinaryNeuralAcidOperationBitNotXor());
    registryOperationType(new BinaryNeuralAcidOperationBitAnd());
    registryOperationType(new BinaryNeuralAcidOperationBitOr());
    m_nOperSize = m_vOperationList.size();
    TAG = "BinaryNeuralAcid";
    m_nBnaVersion = 4;
    m_nBnaRevision = 0;
}

BinaryNeuralAcid::BinaryNeuralAcid(int nInputSize, int nOutputSize) : BinaryNeuralAcid() {
    clearResources();
    for (int i = 0; i < nInputSize; i++) {
        m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(i));
    }
    // for (int i = 0; i < nOutputSize; i++) {
    //     m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(i, i));
    // }
    m_bCompiled = false;
}

unsigned int BinaryNeuralAcid::getInputSize() {
    return m_vNodesInput.size();
}

unsigned int BinaryNeuralAcid::getNodesSize() {
    return m_vNodes.size();
}

unsigned int BinaryNeuralAcid::getOutputSize() {
    return m_vNodesOutput.size();
}

BinaryNeuralAcid::~BinaryNeuralAcid() {
    m_vOperations.clear();
    m_vOperationList.clear();
    m_vNodes.clear();
    clearResources();
}

bool BinaryNeuralAcid::load(const std::string &sFilename){
    clearResources();

    std::string sFilename0 = sFilename + ".bna";
    if (!BinaryNeuralAcid::fileExists(sFilename0)) {
        std::cerr << "load: file not exists '" << sFilename0 << "'" << std::endl;
        return false;
    }
    std::ifstream file;
    file.open(sFilename0, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "load: could not open file to read '" << sFilename0 << "'" << std::endl;
        return false;
    }
    bool bResult = readFromFileBna(file);
    file.close(); // TODO file will be automaticly closed on return of scope?
    return bResult;
}

bool BinaryNeuralAcid::save(const std::string &sFilename){
    std::string sFilename0 = sFilename + ".bna";
    if (BinaryNeuralAcid::fileExists(sFilename0)) {
        if (!BinaryNeuralAcid::removeFile(sFilename0)) {
            std::cerr << "save: could not remove file: '" << sFilename0 << "'" << std::endl;
            return false;
        }
    }
    std::ofstream file;
    file.open(sFilename0, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "save: could not open file to write: '" << sFilename0 << "'" << std::endl;
        return false;
    }

    m_nBnaRevision++;
    bool bResult = writeToFileBna(file);
    file.close(); // TODO file will be automaticly closed on return of scope?
    return bResult;
}

void BinaryNeuralAcid::randomGenerate(const BinaryNeuralAcidConfig &config){
    clearResources();
    m_nBnaRevision = 0;
    for (int i = 0; i < config.getInputSize(); i++) {
        m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(i));
    }
    for (int i = 0; i < config.getNodesSize(); i++) {
        BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
        pItem->setX(rand());
        pItem->setY(rand());
        int nOper = rand() % m_nOperSize;
        pItem->setOperationType(m_vOperationList[nOper]->type());
        m_vNodes.push_back(pItem);
    }
    for (int i = 0; i < config.getOutputSize(); i++) {
        m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(i, rand()));
    }
    compile();
}

int BinaryNeuralAcid::addNode(int nInX, int nInY, const std::string &sOperType) {
    BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
    pItem->setX(nInX);
    pItem->setY(nInY);
    pItem->setOperationType(sOperType);
    m_vNodes.push_back(pItem);
    m_bCompiled = false;
    return m_vNodes.size() - 1 + m_vNodesInput.size();
}

int BinaryNeuralAcid::addNodeOutput(int nNodeId) {
    int nIndex = m_vNodesOutput.size();
    m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(nIndex, nNodeId));
    m_bCompiled = false;
    return nIndex;
}

void BinaryNeuralAcid::clearResources() {
    clearCalcExprsVars();
    // clear input nodes
    for (int i = 0; i < m_vNodesInput.size(); i++) {
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

bool BinaryNeuralAcid::compile() {
    if (m_bCompiled) {
        return true; // already compiled
    }

    // std::cout << "Compiling..." << std::endl;

    clearCalcExprsVars();

    // prepare input nodes
    for (unsigned int i  = 0; i < m_vNodesInput.size(); i++) {
        BinaryNeuralAcidVar<BinaryNeuralAcidBit> *pVar = new BinaryNeuralAcidVar<BinaryNeuralAcidBit>();
        pVar->setValue(B_0);
        pVar->setName("in" + std::to_string(i));
        m_vCalcInputVars.push_back(pVar);
    }

    normalizeNodes();

    int nItemsSize = m_vNodes.size();
    for (int i = 0; i < m_vNodes.size(); i++) {
        BinaryNeuralAcidExpression<BinaryNeuralAcidBit> *pExpr = new BinaryNeuralAcidExpression<BinaryNeuralAcidBit>();
        pExpr->setOperandLeft(getVarByIndex(m_vNodes[i]->getX()));
        pExpr->setOperandRight(getVarByIndex(m_vNodes[i]->getY()));
        pExpr->oper(m_vOperations[m_vNodes[i]->getOperationType()]);
        BinaryNeuralAcidVar<BinaryNeuralAcidBit> *pVar = new BinaryNeuralAcidVar<BinaryNeuralAcidBit>();
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

void BinaryNeuralAcid::compare(BinaryNeuralAcid &bna){
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

bool BinaryNeuralAcid::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

bool BinaryNeuralAcid::removeFile(const std::string &sFilename) {
    return remove(sFilename.c_str()) == 0;
}

bool BinaryNeuralAcid::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
}

bool BinaryNeuralAcid::exportToDot(const std::string &sFilename) {
    std::string sFilename0 = sFilename + ".dot";
    if (BinaryNeuralAcid::fileExists(sFilename0)) {
        if (!BinaryNeuralAcid::removeFile(sFilename0)) {
            std::cerr << "exportToDot: could not remove file: '" << sFilename0 << "'" << std::endl;
            return false;
        }
    }
    std::ofstream file;
    file.open(sFilename0, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "exportToDot: could not open file to write: '" << sFilename0 << "'" << std::endl;
        return false;
    }

    file << "digraph BinaryNeuralAcid {" << "\n";
    for (int i = 0; i < m_vNodesInput.size(); i++) {
        file
            << "    " << i << " [label=\"IN_" << i << "\"]"
            << ";\n";
    }

    for (int i = 0; i < m_vNodes.size(); i++) {
        if (m_vNodes[i]->getOperationType() != "") {
            file
                << "    " << m_vNodes[i]->getId() << " [label=\"" << m_vNodes[i]->getId() << ":" << m_vNodes[i]->getOperationType() << "\"]"
                << ";\n";
        }

        file
            << "    " << m_vNodes[i]->getX()
            << " -> " << m_vNodes[i]->getId()
            << ";\n"
        ;
        file
            << "    " << m_vNodes[i]->getY()
            << " -> " << m_vNodes[i]->getId()
            << ";\n"
        ;

    }
    for (int i = 0; i < m_vNodesOutput.size(); i++) {
        file
            << "    " << "out" << m_vNodesOutput[i]->getOutputIndex() << " [label=\"OUT_" << m_vNodesOutput[i]->getOutputIndex() << "\"]"
            << ";\n";
        file
            << "    " << m_vNodesOutput[i]->getInputNodeIndex()
            << " -> " << "out" << m_vNodesOutput[i]->getOutputIndex()
            << ";\n"
        ;
    }
    file << "}" << "\n";
    return true;
}

// ----------------------------------------------------------------

bool BinaryNeuralAcid::exportToCpp(std::string filename, std::string funcname){
    std::cerr << "TODO exportToCpp" << std::endl;
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

const std::vector<BinaryNeuralAcidGraphNodeInput *> &BinaryNeuralAcid::getNodesInput() {
    return m_vNodesInput;
}

const std::vector<BinaryNeuralAcidGraphNode *> &BinaryNeuralAcid::getNodes() {
    return m_vNodes;
}

const std::vector<BinaryNeuralAcidGraphNodeOutput *> &BinaryNeuralAcid::getNodesOutput() {
    return m_vNodesOutput;
}

int BinaryNeuralAcid::calculateDepth(int n) {
    int nNodesInputSize = m_vNodesInput.size();
    if (n < nNodesInputSize) {
        return 1;
    }
    n = n - nNodesInputSize;
    int nNodesSize = m_vNodes.size();
    if (n < nNodesSize) {
        BinaryNeuralAcidGraphNode *pNode = m_vNodes[n];
        int nLeft = this->calculateDepth(pNode->getX());
        int nRight = this->calculateDepth(pNode->getY());
        return std::max(nLeft, nRight) + 1;
    }
    n = n - nNodesSize;
    int nNodesOutputSize = m_vNodesOutput.size();
    if (n < nNodesOutputSize) {
        BinaryNeuralAcidGraphNodeOutput *pNode = m_vNodesOutput[n];
        return this->calculateDepth(pNode->getInputNodeIndex()) + 1;
    }
    return -1000;
}

bool BinaryNeuralAcid::readFromFileBna(std::ifstream &file){
    clearResources();
    std::string sStr;
    file >> sStr;
    if (sStr != "BinaryNeuralAcid") {
        std::cerr << "readFromFileBna, is not a BinaryNeuralAcid file" << std::endl;
        return false;
    }
    int nBnaVersion = readParam(file, "version");
    if (nBnaVersion != m_nBnaVersion) {
        std::cerr << "readFromFileBna, Version expected '" << m_nBnaVersion << "', but got '" << nBnaVersion << "'" << std::endl;
        return false;
    }
    m_nBnaRevision = readParam(file, "revision");
    int nInputSize = readParam(file, "input");
    int nNodesSize = readParam(file, "nodes");
    int nOutputSize = readParam(file, "output");

    for (int i = 0; i < nInputSize; i++) {
        m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(i));
    }

    // read nodes
    for (int i = 0; i < nNodesSize; i++) {
        BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
        pItem->readFromFile(file);
        m_vNodes.push_back(pItem);
    }

    // read outputs
    for (int i = 0; i < nOutputSize; i++) {
        m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(i, i));
        int nInputNodeIndex = readParam(file, "output");
        m_vNodesOutput[i]->setInputNodeIndex(nInputNodeIndex);
    }

    return compile(); // need for process expressions
}

int BinaryNeuralAcid::readParam(std::ifstream &file, const std::string &sParamName) {
    std::string sKeyword = "";
    file >> sKeyword;
    if (sKeyword != sParamName) {
        throw std::runtime_error("readParam, Expected keyword '" + sParamName + "'");
        return -1;
    }
    int nRet;
    file >> nRet;
    return nRet;
}

bool BinaryNeuralAcid::writeToFileBna(std::ofstream &file){
    // basic information about file
    file << "BinaryNeuralAcid version " << m_nBnaVersion << "\n";
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

bool BinaryNeuralAcid::registryOperationType(IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *pOper) {
    // TODO check aready registered
    m_vOperations[pOper->type()] = pOper;
    m_vOperationList.push_back(pOper);
    return true;
}

void BinaryNeuralAcid::randomModify(const BinaryNeuralAcidModificationModel *pModel) {
    for (int i = 0; i < pModel->getRemoveCicles(); i++) {
        if (m_vNodes.size() == 0) {
            break;
        }
        int nIndex = rand() % m_vNodes.size();
        BinaryNeuralAcidGraphNode *pItem = m_vNodes[nIndex];
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
        BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
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

nlohmann::json BinaryNeuralAcid::toJson() {
    throw std::runtime_error("TODO toJson");
}

// deprected
BinaryNeuralAcidBit BinaryNeuralAcid::calc(const std::vector<BinaryNeuralAcidBit> &vInputs, int nOutput){
    return this->compute(vInputs, nOutput);
}

BinaryNeuralAcidBit BinaryNeuralAcid::compute(const std::vector<BinaryNeuralAcidBit> &vInputs, int nOutput) {
    // prepare calculate exprs
    if (!m_bCompiled) {
        // std::cout << "Not compiled" << std::endl;
        compile();
    }

    if (vInputs.size() != m_vNodesInput.size()) {
        std::cerr << "[ERROR] invalid input count " << vInputs.size() << " (Expected: " << m_vNodesInput.size() << ") \n";
        return B_0;
    }

    for (unsigned int i  = 0; i < m_vNodesInput.size(); i++) {
        m_vCalcInputVars[i]->setValue(vInputs[i]);
    }

    for (unsigned int i = 0; i < m_vCalcExprs.size(); i++) {
        m_vCalcExprs[i]->exec();
    }

    return m_vCalcOutVars[nOutput]->getValue();
}

void BinaryNeuralAcid::clearCalcExprsVars() {

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

void BinaryNeuralAcid::normalizeNodes() {
    int nNodesSize = m_vNodesInput.size();
    // normalize nodes
    for (int i = 0; i < m_vNodes.size(); i++) {
        m_vNodes[i]->setX(m_vNodes[i]->getX() % nNodesSize);
        m_vNodes[i]->setY(m_vNodes[i]->getY() % nNodesSize);
        m_vNodes[i]->setId(i + nNodesSize);
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
            // std::cout << "nNodeIndex= " << nNodeIndex << std::endl;
            vToRemoving.push_back(nNodeIndex);
        }
    }
    // std::cout << "TODO removing all size: " << vToRemoving.size() << std::endl;
    for (int i = vToRemoving.size() - 1; i >= 0; i--) {
        int nNodeIndex = vToRemoving[i];
        int nArrayIndex = vToRemoving[i] - m_vNodesInput.size();
        // std::cout << "Will be removed [" << nNodeIndex << "] in array = " << nArrayIndex << std::endl;
        if (nNodeIndex > m_vNodes.size() + m_vNodesInput.size()) {
            throw std::runtime_error("Node Index very big much");
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

BinaryNeuralAcidVar<BinaryNeuralAcidBit> *BinaryNeuralAcid::getVarByIndex(int nIndex) {
    if (nIndex < m_vCalcInputVars.size()) {
        return m_vCalcInputVars[nIndex];
    }
    nIndex = nIndex - m_vCalcInputVars.size();
    if (nIndex < m_vCalcVars.size()) {
        return m_vCalcVars[nIndex];
    }
    // std::cout << "nIndex = " << nIndex << std::endl;
    // std::cout << "m_vCalcVars.size() = " << m_vCalcVars.size() << std::endl;
    throw std::runtime_error("getVarByIndex: out of range index of var");
    nIndex = nIndex - m_vCalcVars.size();
    return m_vCalcOutVars[nIndex];
}

// ----------------------------------------------------------------
// BinaryNeuralAcidMemoryItem

BinaryNeuralAcidMemoryItem::BinaryNeuralAcidMemoryItem(int nInputBits, int nOutputBits) {
    m_nInputBits = nInputBits;
    m_nOutputBits = nOutputBits;
    TAG = "BinaryNeuralAcidMemoryItem";
}

BinaryNeuralAcidMemory::BinaryNeuralAcidMemory(int nInputBits, int nOutputBits){
    m_nInputBits = nInputBits;
    m_nOutputBits = nOutputBits;
    TAG = "BinaryNeuralAcidMemory";
}

// ----------------------------------------------------------------

void BinaryNeuralAcidMemory::load(std::string filename){
    std::cout << "TODO load" << std::endl;
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
        BinaryNeuralAcidMemoryItem *pBNAMemoryItem = new BinaryNeuralAcidMemoryItem(nInputBits, nOutputBits);
        stream >> pBNAMemoryItem->input;
        stream >> pBNAMemoryItem->output;
        m_vNodes.push_back(pBNAMemoryItem);
    }
    */
};

void BinaryNeuralAcidMemory::save(std::string filename) {
    std::cout << "TODO save" << std::endl;
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

int BinaryNeuralAcidMemory::size(){
    return m_vItems.size();
}

BinaryNeuralAcidMemoryItem* BinaryNeuralAcidMemory::at(int i){
    return m_vItems[i];
}

BinaryNeuralAcidMemoryItem * BinaryNeuralAcidMemory::createItem(){
    return new BinaryNeuralAcidMemoryItem(m_nInputBits, m_nOutputBits);
}

void BinaryNeuralAcidMemory::append(BinaryNeuralAcidMemoryItem *pBNAMemoryItem){
    m_vItems.push_back(pBNAMemoryItem);
}

void BinaryNeuralAcidMemory::printData(){
    std::cerr <<  " --- BinaryNeuralAcid Memory --- \n";
    for (int i = 0; i < m_vItems.size(); i++) {
        std::cerr << "TODO printData" << std::endl;
        // std::cerr << m_vItems[i]->input.toHex().toStdString() << " => " << m_vItems[i]->output.toHex().toStdString() << "\n";
    }
}

// ----------------------------------------------------------------

// void BinaryNeuralAcidMemory::generateData(int nCount){
//     m_vItems.clear();
//     for(int i = 0; i < nCount; i++){
//         BinaryNeuralAcidMemoryItem memoryItem;
//         memoryItem.input.append(generateRandomString());
//         memoryItem.output = QCryptographicHash::hash(memoryItem.input, QCryptographicHash::Md5);
//         m_vItems.push_back(memoryItem);
//     }
// }

// // ----------------------------------------------------------------

// std::string BinaryNeuralAcidMemory::alphabet() {
//     return "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890[]{}:,.<>/?\"'\\*&^%$#!-+=";
// }

// // ----------------------------------------------------------------

// std::string BinaryNeuralAcidMemory::generateRandomString(){
//     std::string sAlphabet = alphabet();
//     int len = qrand() % (m_nInputBits) + 2;
//     std::string str = "";
//     for (int i = 0; i < len; i++) {
//         str += sAlphabet[qrand() % sAlphabet.length()];
//     }
//     return str;
// }

// ----------------------------------------------------------------
// BinaryNeuralAcidProject

BinaryNeuralAcidProject::BinaryNeuralAcidProject(){
    m_nInputBits = 1;
    m_nOutputBits = 1;
    m_nDefaultCountNodes = 5;
    TAG = "BinaryNeuralAcidProject";
}

void BinaryNeuralAcidProject::setInputBits(int nInputBits){
    m_nInputBits = nInputBits;
}

int BinaryNeuralAcidProject::getInputBits(){
    return m_nInputBits;
}

void BinaryNeuralAcidProject::setOutputBits(int nOutputBits){
    m_nOutputBits = nOutputBits;
}

int BinaryNeuralAcidProject::getOutputBits(){
    return m_nOutputBits;
}

void BinaryNeuralAcidProject::setDefaultCountNodes(int nDefaultCountNodes){
    m_nDefaultCountNodes = nDefaultCountNodes;
}

bool BinaryNeuralAcidProject::open(std::string sDirPath){
    if (!BinaryNeuralAcid::dirExists(sDirPath)) {
        std::cerr << "[ERROR] BinaryNeuralAcid Project " << sDirPath << " does not exists in this folder.\n";
        return false;
    }
    m_sDirPath = sDirPath;
    loadProjFile();
    m_pBNAMemory = new BinaryNeuralAcidMemory(m_nInputBits, m_nOutputBits);
    m_pBNAMemory->load(m_sDirPath + "/" + m_sMemoryFileName);

    for (int bitid = 0; bitid < m_nOutputBits; bitid++) {
        std::string m_sBitid = prepareName(bitid);
        std::string subdir = prepareSubdir(bitid);
        std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".bna";
        BinaryNeuralAcid *pBNA = new BinaryNeuralAcid();
        pBNA->load(m_sFilename);
        m_mBNA[bitid] = pBNA;
        m_mapResults[bitid] = loadResult(bitid);
    }

    return true;
}

std::string BinaryNeuralAcidProject::prepareName(int nBitId) {
    std::string sName = std::to_string(nBitId);
    while (sName.length() < 3) {
        sName = "0" + sName;
    }
    return sName;
}

// ----------------------------------------------------------------

std::string BinaryNeuralAcidProject::prepareSubdir(int nBitId){
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

bool BinaryNeuralAcidProject::create(std::string sDirPath){
    std::cerr << "TODO create" << std::endl;

    /*QDir dir(sDirPath);
    if(dir.exists()){
        std::cerr << "[ERROR] BinaryNeuralAcid Project " << sDirPath.toStdString() << " already exists.\n";
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
            BinaryNeuralAcid *pBNA = new BinaryNeuralAcid();
            pBNA->randomGenerate(m_nInputBits,1,m_nDefaultCountNodes);
            pBNA->save(m_sFilename);
            m_mBNA[bitid] = pBNA;
        }
        m_mapResults[bitid] = 0;
        saveResult(bitid, 0);
    }
    m_pBNAMemory = new BinaryNeuralAcidMemory(m_nInputBits, m_nOutputBits);
    m_sMemoryFileName = "data.bnamemory";
    m_pBNAMemory->save(m_sDirPath + "/" + m_sMemoryFileName);
    saveProjFile();
*/
    return true;
}

// ----------------------------------------------------------------

void BinaryNeuralAcidProject::saveProjFile(){
    std::cerr << "TODO saveProjFile" << std::endl;

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

void BinaryNeuralAcidProject::loadProjFile(){
    std::cerr << "TODO loadProjFile" << std::endl;
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

BinaryNeuralAcidMemory *BinaryNeuralAcidProject::getBNAMemory(){
    return m_pBNAMemory;
}

// ----------------------------------------------------------------

void BinaryNeuralAcidProject::saveBNAMemory(){
    m_pBNAMemory->save(m_sDirPath + "/" + m_sMemoryFileName);
}

// ----------------------------------------------------------------

int BinaryNeuralAcidProject::calculate(int nBitId, bool bEnableSleep){

    if(nBitId > m_nOutputBits){
        std::cerr << "Invalid nBitId max possible" << (m_nOutputBits-1) << "\n";
        return 0;
    }
    std::map<int,BinaryNeuralAcid *>::iterator it;
    it = m_mBNA.find(nBitId);
    if (it == m_mBNA.end()) {
        std::cerr << "Not found nBitId in array\n";
        return 0;
    }

    int nResult = 0;
    BinaryNeuralAcid *pBNA = m_mBNA[nBitId];
    int nMemorySize = m_pBNAMemory->size();
    for(int i = 0; i < nMemorySize; i++){
        BinaryNeuralAcidMemoryItem *pBNAMemoryItem = m_pBNAMemory->at(i);
        BinaryNeuralAcidBit bResult; // = pBNA->calc(pBNAMemoryItem->inputToVectorBool(), 0);
        // if (pBNAMemoryItem->outputToVectorBool()[nBitId] == bResult) {
        //     nResult++;
        // }

        if (bEnableSleep && i > 0 && i % 1000 == 0) {
            std::cerr << "TODO sleep here" << std::endl;
            // QThread::sleep(1);
        }
    }
    return nResult;
}

void BinaryNeuralAcidProject::saveResult(int bitid, int nResult){
    std::cerr << "TODO" << std::endl;

    /*std::string m_sBitid = prepareName(bitid);
    std::string subdir = prepareSubdir(bitid);
    std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".result";

    QFile file(m_sFilename);
    if (file.exists()) {
        file.remove();
    }
    if ( !file.open(QIODevice::WriteOnly) ) {
        std::cout << "BinaryNeuralAcidProject/savePersent: Could not write file: " << m_sFilename.toStdString() << "\n";
        return;
    }
    QDataStream stream( &file );
    stream << nResult;
    m_mapResults[bitid] = nResult;
    file.close();*/
}

int BinaryNeuralAcidProject::loadResult(int bitid){
    /*std::string m_sBitid = prepareName(bitid);
    std::string subdir = prepareSubdir(bitid);
    std::string m_sFilename = m_sDirPath + "/" + subdir + "/" + m_sBitid + ".result";

    int nResult = 0;
    // load persent
    QFile file(m_sFilename);
    if (!file.exists()) {
        std::cout << "BinaryNeuralAcidProject/loadPersent:  File did not exists: " << m_sFilename.toStdString() << "\n";
    }else{
        if (!file.open(QIODevice::ReadOnly) ) {
            std::cout << "BinaryNeuralAcidProject/loadPersent: Could not open file " << m_sFilename.toStdString() << "\n";
        }else{
            QDataStream stream(&file);
            stream >> nResult;
        }
    }
    m_mapResults[bitid] = nResult;
    return nResult;*/
}

std::map<int,int> &BinaryNeuralAcidProject::getResults(){
    return m_mapResults;
}

BinaryNeuralAcid *BinaryNeuralAcidProject::getBNA(int nBitId){
    if(nBitId > m_nOutputBits){
        std::cerr << "Invalid bitid max possible" << (m_nOutputBits-1) << "\n";
        return NULL;
    }

    std::map<int, BinaryNeuralAcid *>::iterator it;
    it = m_mBNA.find(nBitId); 
    if (it == m_mBNA.end()) {
        std::cerr << "Not found nBitId in array\n";
        return NULL;
    }

    return m_mBNA[nBitId];
}

// ----------------------------------------------------------------

void BinaryNeuralAcidProject::saveBNA(int nBitId){
    if(nBitId > m_nOutputBits){
        std::cerr << "Invalid nBitId max possible" << (m_nOutputBits-1) << "\n";
        return;
    }
    std::map<int, BinaryNeuralAcid *>::iterator it;
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