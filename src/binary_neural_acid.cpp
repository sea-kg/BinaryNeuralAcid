#include "binary_neural_acid.h"
#include <iostream>
#include <cstring>
#ifndef _MSC_VER
    #include <dirent.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#else
    #include <direct.h>
    #define PATH_MAX 256
#endif
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

std::string BinaryNeuralAcidOperationBitXor::type() {
    return std::string("XOR");
}

BinaryNeuralAcidBit BinaryNeuralAcidOperationBitXor::calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 ^ c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationBitNotXor

std::string BinaryNeuralAcidOperationBitNotXor::type(){
    return std::string("NXOR");
}

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
// BinaryNeuralAcidOperationCharXor

std::string BinaryNeuralAcidOperationCharXor::type() {
    return "XOR";
}

char BinaryNeuralAcidOperationCharXor::calc(char b1, char b2) {
    return b1 ^ b2;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationCharOr

std::string BinaryNeuralAcidOperationCharOr::type() {
    return "OR";
}

char BinaryNeuralAcidOperationCharOr::calc(char b1, char b2) {
    return b1 | b2;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationCharAnd

std::string BinaryNeuralAcidOperationCharAnd::type() {
    return "AND";
}

char BinaryNeuralAcidOperationCharAnd::calc(char b1, char b2) {
    return b1 & b2;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationCharShiftLeft

std::string BinaryNeuralAcidOperationCharShiftLeft::type() {
    return "SHL";
}

char BinaryNeuralAcidOperationCharShiftLeft::calc(char b1, char b2) {
    int shift = b2 % 8;
    char res = (b1 << shift) | (b1 >> (8 - shift));
    return res;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidOperationCharShiftRight

std::string BinaryNeuralAcidOperationCharShiftRight::type() {
    return "SHR";
}

char BinaryNeuralAcidOperationCharShiftRight::calc(char b1, char b2) {
    int shift = b2 % 8;
    char res = (b1 >> shift) | (b1 << (8 - shift));
    return res;
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

// ---------------------------------------------------------------------
// BinaryNeuralAcidPseudoRandomStd

BinaryNeuralAcidPseudoRandomStd::BinaryNeuralAcidPseudoRandomStd() {
    // set seed
    std::srand(std::time(0));
}

unsigned int BinaryNeuralAcidPseudoRandomStd::getNextRandom() {
    // IBinaryNeuralAcidPseudoRandom
    return rand();
}

// ---------------------------------------------------------------------
// BinaryNeuralAcidPseudoRandomSin

BinaryNeuralAcidPseudoRandomSin::BinaryNeuralAcidPseudoRandomSin() {
    this->setInitSeed(1);
}

void BinaryNeuralAcidPseudoRandomSin::setInitSeed(unsigned int nSeed) {
    m_nSeed = nSeed;
    m_nInitSeed = nSeed;
}

unsigned int BinaryNeuralAcidPseudoRandomSin::getNextRandom() {
    // IBinaryNeuralAcidPseudoRandom
    m_nSeed = std::sin(m_nSeed + 1) * float(m_nSeed + 6947504120) + 144;
    return m_nSeed;
}

unsigned int BinaryNeuralAcidPseudoRandomSin::getInitSeed() {
    return m_nInitSeed;
}

unsigned int BinaryNeuralAcidPseudoRandomSin::getSeed() {
    return m_nSeed;
}

// -----------------------------------------------------------------
// BinaryNeuralAcidHelpers

bool BinaryNeuralAcidHelpers::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

bool BinaryNeuralAcidHelpers::removeFile(const std::string &sFilename) {
    return remove(sFilename.c_str()) == 0;
}

bool BinaryNeuralAcidHelpers::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
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

template <>
std::vector<IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *> BinaryNeuralAcidFabricOperations<BinaryNeuralAcidBit>::createOperations() {
    std::vector<IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *> vOperations;
    vOperations.push_back(new BinaryNeuralAcidOperationBitXor());
    vOperations.push_back(new BinaryNeuralAcidOperationBitNotXor());
    vOperations.push_back(new BinaryNeuralAcidOperationBitAnd());
    vOperations.push_back(new BinaryNeuralAcidOperationBitOr());
    return vOperations;
}

template <>
std::vector<IBinaryNeuralAcidOperation<char> *> BinaryNeuralAcidFabricOperations<char>::createOperations() {
    std::vector<IBinaryNeuralAcidOperation<char> *> vOperations;
    vOperations.push_back(new BinaryNeuralAcidOperationCharXor());
    vOperations.push_back(new BinaryNeuralAcidOperationCharOr());
    vOperations.push_back(new BinaryNeuralAcidOperationCharAnd());
    vOperations.push_back(new BinaryNeuralAcidOperationCharShiftLeft());
    vOperations.push_back(new BinaryNeuralAcidOperationCharShiftRight());
    return vOperations;
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
//     int len = m_pRandom->getNextRandom() % (m_nInputBits) + 2;
//     std::string str = "";
//     for (int i = 0; i < len; i++) {
//         str += sAlphabet[m_pRandom->getNextRandom() % sAlphabet.length()];
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
    if (!BinaryNeuralAcidHelpers::dirExists(sDirPath)) {
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
        BinaryNeuralAcid<BinaryNeuralAcidBit> *pBNA = new BinaryNeuralAcid<BinaryNeuralAcidBit>();
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
    std::map<int,BinaryNeuralAcid<BinaryNeuralAcidBit> *>::iterator it;
    it = m_mBNA.find(nBitId);
    if (it == m_mBNA.end()) {
        std::cerr << "Not found nBitId in array\n";
        return 0;
    }

    int nResult = 0;
    BinaryNeuralAcid<BinaryNeuralAcidBit> *pBNA = m_mBNA[nBitId];
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
    return 0;
}

std::map<int,int> &BinaryNeuralAcidProject::getResults(){
    return m_mapResults;
}

BinaryNeuralAcid<BinaryNeuralAcidBit> *BinaryNeuralAcidProject::getBNA(int nBitId){
    if(nBitId > m_nOutputBits){
        std::cerr << "Invalid bitid max possible" << (m_nOutputBits-1) << "\n";
        return NULL;
    }

    std::map<int, BinaryNeuralAcid<BinaryNeuralAcidBit> *>::iterator it;
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
    std::map<int, BinaryNeuralAcid<BinaryNeuralAcidBit> *>::iterator it;
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