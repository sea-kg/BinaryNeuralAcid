#include "bnaoper_test.h"
#include <bna.h>
#include <iostream>

QString BNAOper_Test::name(){
    return "BNAOper_Test";
}

struct BNAOperDataTest{
    BNAOperDataTest(IBNAOper *pOper, BNABit b1, BNABit b2, BNABit bExpected) : pOper(pOper), b1(b1), b2(b2), bExpected(bExpected) {}
    IBNAOper *pOper;
    BNABit b1;
    BNABit b2;
    BNABit bExpected;
};


bool BNAOper_Test::run(){

    bool bError = false;
    BNAOperXor *pOperXor = new BNAOperXor();
    BNAOperNotXor *pOperNotXor = new BNAOperNotXor();
    BNAOperAnd *pOperAnd = new BNAOperAnd();
    BNAOperOr *pOperOr = new BNAOperOr();

    QVector<BNAOperDataTest *> vTests;

    // XOR
    vTests.push_back(new BNAOperDataTest(pOperXor, B_0, B_0, B_0));
    vTests.push_back(new BNAOperDataTest(pOperXor, B_0, B_1, B_1));
    vTests.push_back(new BNAOperDataTest(pOperXor, B_1, B_0, B_1));
    vTests.push_back(new BNAOperDataTest(pOperXor, B_1, B_1, B_0));

    // NXOR
    vTests.push_back(new BNAOperDataTest(pOperNotXor, B_0, B_0, B_1));
    vTests.push_back(new BNAOperDataTest(pOperNotXor, B_0, B_1, B_0));
    vTests.push_back(new BNAOperDataTest(pOperNotXor, B_1, B_0, B_0));
    vTests.push_back(new BNAOperDataTest(pOperNotXor, B_1, B_1, B_1));

    // AND
    vTests.push_back(new BNAOperDataTest(pOperAnd, B_0, B_0, B_0));
    vTests.push_back(new BNAOperDataTest(pOperAnd, B_0, B_1, B_0));
    vTests.push_back(new BNAOperDataTest(pOperAnd, B_1, B_0, B_0));
    vTests.push_back(new BNAOperDataTest(pOperAnd, B_1, B_1, B_1));

    // OR
    vTests.push_back(new BNAOperDataTest(pOperOr, B_0, B_0, B_0));
    vTests.push_back(new BNAOperDataTest(pOperOr, B_0, B_1, B_1));
    vTests.push_back(new BNAOperDataTest(pOperOr, B_1, B_0, B_1));
    vTests.push_back(new BNAOperDataTest(pOperOr, B_1, B_1, B_1));

    for(int i = 0; i < vTests.size(); i++){
        BNAOperDataTest *pTest = vTests[i];
        BNABit b = pTest->pOper->calc(pTest->b1, pTest->b2);
        if(b != pTest->bExpected){
            bError = true;
            std::cerr << "\tWrong "
                      << pTest->b1
                      << " "
                      << pTest->pOper->type().toStdString()
                      << " "
                      << pTest->b2
                      << " == "
                      << b
                      << " Expected "
                      << pTest->bExpected
                      << "\n";
        }
    }

    return !bError;
}

