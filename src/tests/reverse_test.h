#ifndef REVERSE_TEST_H
#define REVERSE_TEST_H

#include "ireversehashtest.h"

class Reverse_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // REVERSE_TEST_H
