#include <bna_expr.h>
#include <QDebug>

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
