#include "unit_test_call_out.h"
#include <vector>
#include <wsjcpp_core.h>
#include <binary_neural_acid.h>

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
    BNA bna(3,1);

    int nodeN1 = bna.addNode(0, 1, "AND");
    compare("nodeN1", nodeN1, 3);
    int nodeN2 = bna.addNode(1, 2, "OR");
    compare("nodeN2", nodeN2, 4);
    int nodeN3 = bna.addNode(nodeN1, nodeN2, "AND");
    compare("nodeN3", nodeN3, 5);
    bna.compile();

    compare("callout_0_0_0", bna.calc({B_0, B_0, B_0}, 0), B_0);
    compare("callout_0_0_1", bna.calc({B_0, B_0, B_1}, 0), B_0);
    compare("callout_0_1_0", bna.calc({B_0, B_1, B_0}, 0), B_0);
    compare("callout_0_1_1", bna.calc({B_0, B_1, B_1}, 0), B_0);
    compare("callout_1_0_0", bna.calc({B_1, B_0, B_0}, 0), B_0);
    compare("callout_1_0_1", bna.calc({B_1, B_0, B_1}, 0), B_0);
    compare("callout_1_1_0", bna.calc({B_1, B_1, B_0}, 0), B_1);
    compare("callout_1_1_1", bna.calc({B_1, B_1, B_1}, 0), B_1);


    WsjcppCore::makeDir("./temporary-unit-tests-data");
    
    bool bSave0 = bna.save("./temporary-unit-tests-data/callout-test0");
    compare("save0", bSave0, true);

    compare("callout_after_save_0_0_0", bna.calc({B_0, B_0, B_0}, 0), B_0);
    compare("callout_after_save_0_0_1", bna.calc({B_0, B_0, B_1}, 0), B_0);
    compare("callout_after_save_0_1_0", bna.calc({B_0, B_1, B_0}, 0), B_0);
    compare("callout_after_save_0_1_1", bna.calc({B_0, B_1, B_1}, 0), B_0);
    compare("callout_after_save_1_0_0", bna.calc({B_1, B_0, B_0}, 0), B_0);
    compare("callout_after_save_1_0_1", bna.calc({B_1, B_0, B_1}, 0), B_0);
    compare("callout_after_save_1_1_0", bna.calc({B_1, B_1, B_0}, 0), B_1);
    compare("callout_after_save_1_1_1", bna.calc({B_1, B_1, B_1}, 0), B_1);

    bool bLoad1 = bna.load("./temporary-unit-tests-data/callout-test0");
    compare("load1", bLoad1, true);

    compare("callout_after_load_0_0_0", bna.calc({B_0, B_0, B_0}, 0), B_0);
    compare("callout_after_load_0_0_1", bna.calc({B_0, B_0, B_1}, 0), B_0);
    compare("callout_after_load_0_1_0", bna.calc({B_0, B_1, B_0}, 0), B_0);
    compare("callout_after_load_0_1_1", bna.calc({B_0, B_1, B_1}, 0), B_0);
    compare("callout_after_load_1_0_0", bna.calc({B_1, B_0, B_0}, 0), B_0);
    compare("callout_after_load_1_0_1", bna.calc({B_1, B_0, B_1}, 0), B_0);
    compare("callout_after_load_1_1_0", bna.calc({B_1, B_1, B_0}, 0), B_1);
    compare("callout_after_load_1_1_1", bna.calc({B_1, B_1, B_1}, 0), B_1);

    bool bSave1 = bna.save("./temporary-unit-tests-data/callout-test1");
    compare("save1", bSave1, true);
}

// ---------------------------------------------------------------------

bool UnitTestCallOut::doAfterTest() {
    return true;
}