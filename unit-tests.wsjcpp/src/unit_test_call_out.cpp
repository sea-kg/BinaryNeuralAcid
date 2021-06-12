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
    BNA bna(3,1);

    int nodeN1 = bna.addItem(0, 1, "AND");
    compare("nodeN1", nodeN1, 3);
    int nodeN2 = bna.addItem(1, 2, "OR");
    compare("nodeN2", nodeN2, 4);
    int nodeN3 = bna.addItem(nodeN1, nodeN2, "OR");
    compare("nodeN3", nodeN3, 5);

    compare("callout_0_0_0", bna.calc({B_0, B_0, B_0}, 0), B_0);
    compare("callout_0_0_1", bna.calc({B_0, B_0, B_1}, 0), B_0);
    compare("callout_0_1_0", bna.calc({B_0, B_1, B_0}, 0), B_0);
    compare("callout_0_1_1", bna.calc({B_0, B_1, B_1}, 0), B_0);
    compare("callout_1_0_0", bna.calc({B_1, B_0, B_0}, 0), B_0);
    compare("callout_1_0_1", bna.calc({B_1, B_0, B_1}, 0), B_0);
    compare("callout_1_1_0", bna.calc({B_1, B_1, B_0}, 0), B_1);
    compare("callout_1_1_1", bna.calc({B_1, B_1, B_1}, 0), B_1);
}

// ---------------------------------------------------------------------

bool UnitTestCallOut::doAfterTest() {
    return true;
}