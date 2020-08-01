#include "unit_test_read_write.h"
#include <vector>
#include <wsjcpp_core.h>
#include <bna.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestReadWrite)

UnitTestReadWrite::UnitTestReadWrite()
    : WsjcppUnitTestBase("UnitTestReadWrite") {
}

// ---------------------------------------------------------------------

void UnitTestReadWrite::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestReadWrite::run() {
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
    compareB(bTestSuccess, "save0", bSave0, true);

    bool bLoad0 = bna.load("./temporary-unit-tests-data/read-write-test0");
    compareB(bTestSuccess, "load0", bLoad0, true);

    compareN(bTestSuccess, "test read write 0", bResult, bna.calc(vInputs, 0));

    bna.save("./temporary-unit-tests-data/read-write-test1");
    bna.load("./temporary-unit-tests-data/read-write-test1");

    compareN(bTestSuccess, "test read write 1", bResult, bna.calc(vInputs, 0));

    return bTestSuccess;
}

