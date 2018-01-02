#ifndef LEARNING_TEST_H
#define LEARNING_TEST_H

#include "ireversehashtest.h"

class Learning_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // LEARNING_TEST_H
