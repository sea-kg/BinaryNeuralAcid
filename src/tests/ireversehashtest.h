#ifndef IREVERSEHASHTEST_H
#define IREVERSEHASHTEST_H

#include <QString>

class IReverseHashTest {
	public:
		virtual QString name() = 0;
		virtual bool run() = 0;
};

#endif // IREVERSEHASHTEST_H
