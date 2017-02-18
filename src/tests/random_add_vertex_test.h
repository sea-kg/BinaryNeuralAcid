#ifndef RANDOM_ADD_VERTEX_TEST_H
#define RANDOM_ADD_VERTEX_TEST_H

#include "ireversehashtest.h"

class RandomAddVertex_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
	private:
		void printStat(int i, int cicles, int max);
};

#endif // RANDOM_ADD_VERTEX_TEST_H
