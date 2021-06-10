#ifndef UNIT_TEST_LEARNING_H
#define UNIT_TEST_LEARNING_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestLearning : public WsjcppUnitTestBase {
    public:
        UnitTestLearning();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_LEARNING_H

