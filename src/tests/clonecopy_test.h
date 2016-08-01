#ifndef CLONECOPY_TEST_H
#define CLONECOPY_TEST_H

#include "ireversehashtest.h"

class CloneCopy_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // CLONECOPY_TEST_H
