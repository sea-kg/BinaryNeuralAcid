#ifndef UNIT_TEST_BNA_OPER_H
#define UNIT_TEST_BNA_OPER_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestBnaOper : public WsjcppUnitTestBase {
    public:
        UnitTestBnaOper();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_BNA_OPER_H

