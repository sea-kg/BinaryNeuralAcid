#include "unit_test_read_write.h"
#include <vector>
#include <wsjcpp_core.h>
#include <bna.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestReadWrite)

UnitTestReadWrite::UnitTestReadWrite()
    : WsjcppUnitTestBase("UnitTestReadWrite") {
}

// ---------------------------------------------------------------------

bool UnitTestReadWrite::doBeforeTest() {
    return false;
}

// ---------------------------------------------------------------------

void UnitTestReadWrite::executeTest() {
    bool bTestSuccess = true;
    WsjcppCore::makeDir("./temporary-unit-tests-data");

    
    BNA bna;
    bna.randomGenerate(128,1,256);
    
    // fill the input vector
    std::vector<BNABit> vInputs;
    for (int i = 0; i < 128; i++) {
        vInputs.push_back(B_1);
    }
    BNABit bResult = bna.calc(vInputs, 0);

    bool bSave0 = bna.save("./temporary-unit-tests-data/read-write-test0");
    compare("save0", bSave0, true);

    bool bLoad0 = bna.load("./temporary-unit-tests-data/read-write-test0");
    compare("load0", bLoad0, true);

    compare("test read write 0", bResult, bna.calc(vInputs, 0));

    bool bSave1 = bna.save("./temporary-unit-tests-data/read-write-test1");
    compare("save1", bSave0, true);

    bool bLoad1 = bna.load("./temporary-unit-tests-data/read-write-test1");
    compare("load1", bLoad1, true);

    compare("test read write 1", bResult, bna.calc(vInputs, 0));
}

// ---------------------------------------------------------------------

bool UnitTestReadWrite::doAfterTest() {
    return false;
}