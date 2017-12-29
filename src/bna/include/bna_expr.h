#ifndef BNA_EXPR_H
#define BNA_EXPR_H

#include <bna_var.h>
#include <QString>

class BNAExpr{
public:
    BNAExpr();
    void op1(BNAVar *pVar1);
    void op2(BNAVar *pVar2);
    // todo oper
    void out(BNAVar *pVarOut);
    void exec();

private:
    BNAVar *m_pVar1;
    BNAVar *m_pVar2;
    BNAVar *m_pVarOut;
};

#endif // BNA_EXPR_H
