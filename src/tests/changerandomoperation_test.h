#ifndef CHANGERANDOMOPERATION_TEST_H
#define CHANGERANDOMOPERATION_TEST_H

#include "ireversehashtest.h"

class ChangeRandomOperation_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // CHANGERANDOMOPERATION_TEST_H
