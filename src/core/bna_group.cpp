#include "bna_group.h"
#include <iostream>
#include <iostream>
#include <cstring>
#include <wsjcpp_core.h>


// -----------------------------------------------------------------
// BNA

BNAGroup::BNAGroup() {
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

BNAGroup::BNAGroup(int nInputSize, int nOutputSize) : BNAGroup() {
    for (int i = 0; i < nInputSize; i++) {
        m_vNodesInput.push_back(new BNANodeInput(i));
    }
    for (int i = 0; i < nOutputSize; i++) {
        m_vNodesOutput.push_back(new BNANodeOutput(i, i));
    }
    m_bCompiled = false;
}

unsigned int BNAGroup::getInputSize() {
    return m_vNodesInput.size();
}

unsigned int BNAGroup::getNodesSize() {
    return m_vNodes.size();
}

unsigned int BNAGroup::getOutputSize() {
    return m_vNodesOutput.size();
}

BNAGroup::~BNAGroup() {
    m_vOperations.clear();
    m_vOperationList.clear();
    m_vNodes.clear();
    clearResources();
}

// ----------------------------------------------------------------

bool BNAGroup::load(const std::string &sFilename){
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

bool BNAGroup::save(const std::string &sFilename){
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

void BNAGroup::randomGenerate(int nInputSize, int nOutputSize, int nSize){
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

int BNAGroup::addNode(int nInX, int nInY, const std::string &sOperType) {
    BNANode *pItem = new BNANode();
    pItem->setX(nInX);
    pItem->setY(nInY);
    pItem->setOperationType(sOperType);
    m_vNodes.push_back(pItem);
    m_bCompiled = false;
    return m_vNodes.size() - 1 + m_vNodesInput.size();
}

void BNAGroup::clearResources() {
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

bool BNAGroup::compile() {
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

void BNAGroup::compare(BNAGroup &bna){
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

bool BNAGroup::exportToDot(std::string filename, std::string graphname){
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

bool BNAGroup::exportToCpp(std::string filename, std::string funcname){
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

const std::vector<BNANodeInput *> &BNAGroup::getNodesInput() {
    return m_vNodesInput;
}

const std::vector<BNANode *> &BNAGroup::getNodes() {
    return m_vNodes;
}

const std::vector<BNANodeOutput *> &BNAGroup::getNodesOutput() {
    return m_vNodesOutput;
}

int BNAGroup::calculateDepth(int n) {
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

bool BNAGroup::readFromFileBna(std::ifstream &file){
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

int BNAGroup::readParam(std::ifstream &file, const std::string &sParamName) {
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

bool BNAGroup::writeToFileBna(std::ofstream &file){
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

bool BNAGroup::registryOperationType(IBNAOper<BNABit> *pOper) {
    // TODO check aready registered
    m_vOperations[pOper->type()] = pOper;
    m_vOperationList.push_back(pOper);
    return true;
}

void BNAGroup::randomModify(const BNAModificationModel *pModel) {
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

nlohmann::json BNAGroup::toJson(){
    WsjcppLog::err(TAG, "TODO toJson");
}

BNABit BNAGroup::calc(const std::vector<BNABit> &vInputs, int nOutput){
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

void BNAGroup::clearCalcExprsVars() {
    
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

void BNAGroup::normalizeNodes() {
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

BNAVar<BNABit> *BNAGroup::getVarByIndex(int nIndex) {
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
