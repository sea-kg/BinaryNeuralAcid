#include "bna_expression.h"
#include <iostream>

// -----------------------------------------------------------------
// BNA Expression class for calculation by operation

BNAExpression::BNAExpression() {
    m_pVar1 = nullptr;
    m_pVar2 = nullptr;
    m_pVarOut = nullptr;
    m_pOper = nullptr;
}

void BNAExpression::setOperandLeft(BNAVar *pVar1) {
    m_pVar1 = pVar1;
}

BNAVar *BNAExpression::getOperandLeft() {
    return m_pVar1;
}

void BNAExpression::setOperandRight(BNAVar *pVar2) {
    m_pVar2 = pVar2; 
}

BNAVar *BNAExpression::op2() {
    return m_pVar2;
}

void BNAExpression::oper(IBNAOper *pOper) {
    m_pOper = pOper;
}

IBNAOper *BNAExpression::oper() {
    return m_pOper;
}

void BNAExpression::out(BNAVar *pVarOut) {
    m_pVarOut = pVarOut;
}

BNAVar * BNAExpression::out() {
    return m_pVarOut;
}

void BNAExpression::exec(){
    if(m_pVar1 == nullptr){
        std::cerr << "[ERROR] m_pVar1 is NULL" << std::endl;
        return;
    }

    if(m_pVar2 == nullptr){
        std::cerr << "[ERROR] m_pVar2 is NULL" << std::endl;
        return;
    }

    if(m_pVarOut == nullptr){
        std::cerr << "[ERROR] m_pVarOut is NULL\n" << std::endl;
        return;
    }

    if(m_pOper == nullptr){
        std::cerr << "[ERROR] m_pOper is NULL" << std::endl;
        return;
    }
    BNABit b1 = m_pVar1->val();
    BNABit b2 = m_pVar2->val();
    m_pVarOut->val(m_pOper->calc(b1, b2));
}