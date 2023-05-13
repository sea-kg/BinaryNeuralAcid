#include "binary_neural_acid.h"

#include <vector>
#include <iostream>


int main() {
    BNAOperXor *pOperXor = new BNAOperXor();
    BNAOperNotXor *pOperNotXor = new BNAOperNotXor();
    BNAOperAnd *pOperAnd = new BNAOperAnd();
    BNAOperOr *pOperOr = new BNAOperOr();

    struct LocalDataTest {
        LocalDataTest(IBNAOper<BNABit> *pOper, BNABit b1, BNABit b2, BNABit bExpected) {
            m_pOper = pOper;
            m_b1 = b1;
            m_b2 = b2;
            m_bExpected = bExpected;
        }
        IBNAOper<BNABit> *m_pOper;
        BNABit m_b1;
        BNABit m_b2;
        BNABit m_bExpected;
        BNABit calc() {
            return m_pOper->calc(m_b1, m_b2);
        }
    };

    std::vector<LocalDataTest> vTests;

    // XOR
    vTests.push_back(LocalDataTest(pOperXor, B_0, B_0, B_0));
    vTests.push_back(LocalDataTest(pOperXor, B_0, B_1, B_1));
    vTests.push_back(LocalDataTest(pOperXor, B_1, B_0, B_1));
    vTests.push_back(LocalDataTest(pOperXor, B_1, B_1, B_0));

    // NXOR
    vTests.push_back(LocalDataTest(pOperNotXor, B_0, B_0, B_1));
    vTests.push_back(LocalDataTest(pOperNotXor, B_0, B_1, B_0));
    vTests.push_back(LocalDataTest(pOperNotXor, B_1, B_0, B_0));
    vTests.push_back(LocalDataTest(pOperNotXor, B_1, B_1, B_1));

    // AND
    vTests.push_back(LocalDataTest(pOperAnd, B_0, B_0, B_0));
    vTests.push_back(LocalDataTest(pOperAnd, B_0, B_1, B_0));
    vTests.push_back(LocalDataTest(pOperAnd, B_1, B_0, B_0));
    vTests.push_back(LocalDataTest(pOperAnd, B_1, B_1, B_1));

    // OR
    vTests.push_back(LocalDataTest(pOperOr, B_0, B_0, B_0));
    vTests.push_back(LocalDataTest(pOperOr, B_0, B_1, B_1));
    vTests.push_back(LocalDataTest(pOperOr, B_1, B_0, B_1));
    vTests.push_back(LocalDataTest(pOperOr, B_1, B_1, B_1));

    for (int i = 0; i < vTests.size(); i++) {
        LocalDataTest test = vTests[i];
        BNABit b = test.calc();
        std::string sTestTag = std::to_string(i) + " " + test.m_pOper->type();
        if (b != test.m_bExpected) {
            return 1;
        }
    }
    return 0;
}