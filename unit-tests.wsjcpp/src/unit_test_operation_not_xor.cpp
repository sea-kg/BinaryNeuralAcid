
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>
#include <bna_operations.h>

// ---------------------------------------------------------------------
// UnitTestOperationNotXor

class UnitTestOperationNotXor : public WsjcppUnitTestBase {
    public:
        UnitTestOperationNotXor();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestOperationNotXor)

UnitTestOperationNotXor::UnitTestOperationNotXor()
    : WsjcppUnitTestBase("UnitTestOperationNotXor") {
}

// ---------------------------------------------------------------------

bool UnitTestOperationNotXor::doBeforeTest() {
    // do something before test
    return true;
}

// ---------------------------------------------------------------------

void UnitTestOperationNotXor::executeTest() {
    IBNAOper *pOper = new BNAOperNotXor();
    compare("oper_notxor_name", pOper->type(), "NXOR");
    compare("oper_notxor_0_0", pOper->calc(BNABit::B_0, BNABit::B_0), BNABit::B_1);
    compare("oper_notxor_0_1", pOper->calc(BNABit::B_0, BNABit::B_1), BNABit::B_0);
    compare("oper_notxor_1_0", pOper->calc(BNABit::B_1, BNABit::B_0), BNABit::B_0);
    compare("oper_notxor_1_1", pOper->calc(BNABit::B_1, BNABit::B_1), BNABit::B_1);
}

// ---------------------------------------------------------------------

bool UnitTestOperationNotXor::doAfterTest() {
    // do somethig after test
    return true;
}


