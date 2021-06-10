#ifndef UNIT_TEST_READ_WRITE_H
#define UNIT_TEST_READ_WRITE_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestReadWrite : public WsjcppUnitTestBase {
    public:
        UnitTestReadWrite();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_READ_WRITE_H

