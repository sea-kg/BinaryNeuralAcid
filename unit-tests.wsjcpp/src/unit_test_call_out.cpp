#include "unit_test_call_out.h"
#include <vector>
#include <wsjcpp_core.h>
#include <bna.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestCallOut)

UnitTestCallOut::UnitTestCallOut()
    : WsjcppUnitTestBase("UnitTestCallOut") {
}

// ---------------------------------------------------------------------

bool UnitTestCallOut::doBeforeTest() {
    return true;
}

// ---------------------------------------------------------------------

void UnitTestCallOut::executeTest() {
    BNA bna;

    bna.randomGenerate(15, 1, 100);
    std::vector<BNABit> vInputs;
    int nPositiveOutputs = 0;
    int nNegativeOutputs = 0;

    for (int t = 0; t < 1500; t++) {
        vInputs.clear();
        for (int i = 0; i < 15; i++) {
            vInputs.push_back(t*i % 2 == 1 ?  B_1 : B_0);
        }
        if (bna.calc(vInputs, 0) == B_1) {
            nPositiveOutputs++;
        }
        if (bna.calc(vInputs, 0) == B_0) {
            nNegativeOutputs++;
        }
    }

    compare("random_positive", nPositiveOutputs != 0, true);
    compare("random_negative", nNegativeOutputs != 0, true);
}

// ---------------------------------------------------------------------

bool UnitTestCallOut::doAfterTest() {
    return true;
}