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
    return false;
}

// ---------------------------------------------------------------------

void UnitTestCallOut::executeTest() {
    BNA bna;
    
    // TODO  no random generate
    bna.randomGenerate(15, 1, 100);
    std::vector<BNABit> vInputs;

    for (int i = 0; i < 15; i++) {
        vInputs.push_back(B_1);
    }
    bna.calc(vInputs, 0);
}

// ---------------------------------------------------------------------

bool UnitTestCallOut::doAfterTest() {
    return true;
}