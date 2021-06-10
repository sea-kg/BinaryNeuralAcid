#ifndef UNIT_TEST_BRUTE_H
#define UNIT_TEST_BRUTE_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestBrute : public WsjcppUnitTestBase {
    public:
        UnitTestBrute();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_BRUTE_H

