#ifndef BNAOPER_TEST_H
#define BNAOPER_TEST_H

#include "ireversehashtest.h"

class BNAOper_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // BNAOPER_TEST_H
