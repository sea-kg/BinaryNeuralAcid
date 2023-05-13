
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>
#include <bna_operations.h>

// ---------------------------------------------------------------------
// UnitTestOperationOr

class UnitTestOperationOr : public WsjcppUnitTestBase {
    public:
        UnitTestOperationOr();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestOperationOr)

UnitTestOperationOr::UnitTestOperationOr()
    : WsjcppUnitTestBase("UnitTestOperationOr") {
}

// ---------------------------------------------------------------------

bool UnitTestOperationOr::doBeforeTest() {
    // do something before test
    return true;
}

// ---------------------------------------------------------------------

void UnitTestOperationOr::executeTest() {
    IBNAOper *pOper = new BNAOperOr();
    compare("oper_or_name", pOper->type(), "OR");
    compare("oper_or_0_0", pOper->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0), BinaryNeuralAcidBit::B_0);
    compare("oper_or_0_1", pOper->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1), BinaryNeuralAcidBit::B_1);
    compare("oper_or_1_0", pOper->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0), BinaryNeuralAcidBit::B_1);
    compare("oper_or_1_1", pOper->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1), BinaryNeuralAcidBit::B_1);
}

// ---------------------------------------------------------------------

bool UnitTestOperationOr::doAfterTest() {
    // do somethig after test
    return true;
}


