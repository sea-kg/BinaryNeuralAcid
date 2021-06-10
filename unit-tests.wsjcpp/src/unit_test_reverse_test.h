#ifndef UNIT_TEST_REVERSE_TEST_H
#define UNIT_TEST_REVERSE_TEST_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestReverseTest : public WsjcppUnitTestBase {
    public:
        UnitTestReverseTest();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_REVERSE_TEST_H

