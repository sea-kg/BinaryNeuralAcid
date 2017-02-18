#ifndef SWAPRANDOMVERTEXINS_TEST_H
#define SWAPRANDOMVERTEXINS_TEST_H

#include "ireversehashtest.h"

class SwapRandomVertexIns_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
	private:
		void printStat(int i, int cicles, int max);
};

#endif // SWAPRANDOMVERTEXINS_TEST_H
