#ifndef BRUT_TEST_H
#define BRUT_TEST_H

#include "ireversehashtest.h"

class Brut_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
};

#endif // BRUT_TEST_H
