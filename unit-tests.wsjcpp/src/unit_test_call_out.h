#ifndef UNIT_TEST_CALL_OUT_H
#define UNIT_TEST_CALL_OUT_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestCallOut : public WsjcppUnitTestBase {
    public:
        UnitTestCallOut();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_CALL_OUT_H

