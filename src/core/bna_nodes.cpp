#include "bna_nodes.h"
#include <iostream>
#include <iostream>
#include <cstring>
#include <wsjcpp_core.h>

// -----------------------------------------------------------------
// BNANode 

BNANode::BNANode(unsigned short x, unsigned short y, const std::string &sOperationType){
    TAG = "BNANode";
    m_nX = x;
    m_nY = y;
    m_nId = 0;
    m_sOperationType = sOperationType;
}

BNANode::BNANode() {
    TAG = "BNANode";
    m_nX = 0;
    m_nY = 0;
    m_nId = 0;
    m_sOperationType = "";
}

unsigned short BNANode::getX(){
    return m_nX;
}

void BNANode::setX(unsigned short nX){
    m_nX = nX;
}

unsigned short BNANode::getY(){
    return m_nY;
}

void BNANode::setY(unsigned short nY){
    m_nY = nY;
}

std::string BNANode::getOperationType(){
    return m_sOperationType;
}

void BNANode::setOperationType(const std::string &sOperationType){
    m_sOperationType = sOperationType;
}

unsigned short BNANode::getId(){
    return m_nId;
}

void BNANode::setId(unsigned short nId){
    m_nId = nId;
}

void BNANode::readFromFile(std::ifstream &file){
    std::string sKeyword = "";
    file >> sKeyword;
    if (sKeyword != "node") {
        WsjcppLog::throw_err(TAG, "BNANode::readFromFile, Expected keyword 'node'");
    }
    file >> sKeyword;
    if (sKeyword != "id") {
        WsjcppLog::throw_err(TAG, "BNANode::readFromFile, Expected keyword 'id'");
    }
    file >> m_nId;
    file >> sKeyword;
    if (sKeyword != "input") {
        WsjcppLog::throw_err(TAG, "BNANode::readFromFile, Expected keyword 'input'");
    }
    file >> m_nX;
    file >> m_nY;
    file >> sKeyword;
    if (sKeyword != "function") {
        WsjcppLog::throw_err(TAG, "BNANode::readFromFile, Expected keyword 'function'");
    }
    file >> m_sOperationType;
}

void BNANode::writeToFile(std::ofstream &file){
    file << "node id " << m_nId << " input " << m_nX << " " << m_nY << " function " << m_sOperationType << "\n";
}

// -----------------------------------------------------------------
// BNANodeInput

BNANodeInput::BNANodeInput(unsigned short nIndex){
    m_nIndex = nIndex;
}

unsigned short BNANodeInput::getIndex() {
    return m_nIndex;
}

// -----------------------------------------------------------------
// BNANodeOutput

BNANodeOutput::BNANodeOutput(unsigned short nOutputIndex, unsigned short nInputNodeIndex){
    m_nOutputIndex = nOutputIndex;
    m_nInputNodeIndex = nInputNodeIndex;
}

unsigned short BNANodeOutput::getOutputIndex() {
    return m_nOutputIndex;
}

unsigned short BNANodeOutput::getInputNodeIndex() {
    return m_nInputNodeIndex;
}

void BNANodeOutput::setInputNodeIndex(unsigned short nInputNodeIndex) {
    m_nInputNodeIndex = nInputNodeIndex;
}

void BNANodeOutput::writeToFile(std::ofstream &file) {
    file << "output " << m_nInputNodeIndex << "\n";
}
