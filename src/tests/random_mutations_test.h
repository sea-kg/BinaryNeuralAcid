#ifndef RANDOM_MUTATIONS_TEST_H
#define RANDOM_MUTATIONS_TEST_H

#include "ireversehashtest.h"

class RandomMutations_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
	private:
		void printStat(int i, int cicles, int max);
};

#endif // RANDOM_MUTATIONS_TEST_H
