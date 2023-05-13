
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>
#include <bna_operations.h>

// ---------------------------------------------------------------------
// UnitTestOperationXor

class UnitTestOperationXor : public WsjcppUnitTestBase {
    public:
        UnitTestOperationXor();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestOperationXor)

UnitTestOperationXor::UnitTestOperationXor()
    : WsjcppUnitTestBase("UnitTestOperationXor") {
}

// ---------------------------------------------------------------------

bool UnitTestOperationXor::doBeforeTest() {
    // do something before test
    return true;
}

// ---------------------------------------------------------------------

void UnitTestOperationXor::executeTest() {
    IBNAOper *pOper = new BNAOperXor();
    compare("oper_xor_name", pOper->type(), "XOR");
    compare("oper_xor_0_0", pOper->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0), BinaryNeuralAcidBit::B_0);
    compare("oper_xor_0_1", pOper->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1), BinaryNeuralAcidBit::B_1);
    compare("oper_xor_1_0", pOper->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0), BinaryNeuralAcidBit::B_1);
    compare("oper_xor_1_1", pOper->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1), BinaryNeuralAcidBit::B_0);
}

// ---------------------------------------------------------------------

bool UnitTestOperationXor::doAfterTest() {
    // do somethig after test
    return true;
}


