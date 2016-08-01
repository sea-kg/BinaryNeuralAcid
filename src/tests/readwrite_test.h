#ifndef READWRITE_TEST_H
#define READWRITE_TEST_H

#include "ireversehashtest.h"

class ReadWrite_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // READWRITE_TEST_H
