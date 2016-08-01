#ifndef CALLOUT_TEST_H
#define CALLOUT_TEST_H

#include "ireversehashtest.h"

class CallOut_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // CALLOUT_TEST_H
