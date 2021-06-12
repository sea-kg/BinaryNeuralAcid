
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>
#include <bna_operations.h>

// ---------------------------------------------------------------------
// UnitTestOperationAnd

class UnitTestOperationAnd : public WsjcppUnitTestBase {
    public:
        UnitTestOperationAnd();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestOperationAnd)

UnitTestOperationAnd::UnitTestOperationAnd()
    : WsjcppUnitTestBase("UnitTestOperationAnd") {
}

// ---------------------------------------------------------------------

bool UnitTestOperationAnd::doBeforeTest() {
    // do something before test
    return true;
}

// ---------------------------------------------------------------------

void UnitTestOperationAnd::executeTest() {
    IBNAOper *pOper = new BNAOperAnd();
    compare("oper_and_name", pOper->type(), "AND");
    compare("oper_and_0_0", pOper->calc(BNABit::B_0, BNABit::B_0), BNABit::B_0);
    compare("oper_and_0_1", pOper->calc(BNABit::B_0, BNABit::B_1), BNABit::B_0);
    compare("oper_and_1_0", pOper->calc(BNABit::B_1, BNABit::B_0), BNABit::B_0);
    compare("oper_and_1_1", pOper->calc(BNABit::B_1, BNABit::B_1), BNABit::B_1);
}

// ---------------------------------------------------------------------

bool UnitTestOperationAnd::doAfterTest() {
    // do somethig after test
    return true;
}


