#ifndef RANDOM_REMOVE_VERTEX_TEST_H
#define RANDOM_REMOVE_VERTEX_TEST_H

#include "ireversehashtest.h"

class RandomRemoveVertex_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
	private:
		void printStat(int i, int cicles, int max);
};

#endif // RANDOM_REMOVE_VERTEX_TEST_H
