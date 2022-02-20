#ifndef SEA5KG_BNA_EXPRESSION_H
#define SEA5KG_BNA_EXPRESSION_H

#include "bna_types.h"
#include "bna_operations.h"

class BNAExpression {
    public:
        BNAExpression();
        void setOperandLeft(BNAVar *pVar1);
        BNAVar *getOperandLeft();
        void setOperandRight(BNAVar *pVar2);
        BNAVar *op2();
        void oper(IBNAOper *pOper);
        IBNAOper *oper();
        void out(BNAVar *pVarOut);
        BNAVar *out();
        void exec();

    private:
        BNAVar *m_pVar1;
        BNAVar *m_pVar2;
        BNAVar *m_pVarOut;
        IBNAOper *m_pOper;
};

#endif // SEA5KG_BNA_EXPRESSION_H