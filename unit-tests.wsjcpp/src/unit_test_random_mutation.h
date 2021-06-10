#ifndef UNIT_TEST_RANDOM_MUTATION_H
#define UNIT_TEST_RANDOM_MUTATION_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestRandomMutation : public WsjcppUnitTestBase {
    public:
        UnitTestRandomMutation();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_RANDOM_MUTATION_H

