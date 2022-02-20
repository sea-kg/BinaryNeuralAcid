#ifndef BNA_EXPRESSION_H
#define BNA_EXPRESSION_H

#include <iostream>

#include "bna_types.h"
#include "bna_operations.h"

template<class ValueType> class BNAExpression {
    public:
        BNAExpression() {
            m_pVar1 = nullptr;
            m_pVar2 = nullptr;
            m_pVarOut = nullptr;
            m_pOper = nullptr;
        }
        void setOperandLeft(BNAVar<ValueType> *pVar1) {
            m_pVar1 = pVar1;
        }
        BNAVar<ValueType> *getOperandLeft() {
            return m_pVar1;
        }
        void setOperandRight(BNAVar<ValueType> *pVar2) {
            m_pVar2 = pVar2;
        }
        BNAVar<ValueType> *op2() {
            return m_pVar2;
        }
        void oper(IBNAOper<ValueType> *pOper) {
            m_pOper = pOper;
        }
        IBNAOper<ValueType> *oper() {
            return m_pOper;
        }
        void out(BNAVar<ValueType> *pVarOut) {
            m_pVarOut = pVarOut;
        }
        BNAVar<ValueType> *out() {
            return  m_pVarOut;
        }
        void exec() {
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
            ValueType b1 = m_pVar1->getValue();
            ValueType b2 = m_pVar2->getValue();
            m_pVarOut->setValue(m_pOper->calc(b1, b2));
        }

    private:
        BNAVar<ValueType> *m_pVar1;
        BNAVar<ValueType> *m_pVar2;
        BNAVar<ValueType> *m_pVarOut;
        IBNAOper<ValueType> *m_pOper;
};

#endif // BNA_EXPRESSION_H