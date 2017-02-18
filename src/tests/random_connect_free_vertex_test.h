#ifndef RANDOM_CONNECT_FREE_VERTEX_TEST_H
#define RANDOM_CONNECT_FREE_VERTEX_TEST_H

#include "ireversehashtest.h"

class RandomConnectFreeVertex_Test : public IReverseHashTest {
	public:
		virtual QString name();
		virtual bool run();
	private:
		void printStat(int i, int cicles, int max);
};

#endif // RANDOM_CONNECT_FREE_VERTEX_TEST_H
