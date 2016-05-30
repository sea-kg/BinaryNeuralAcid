#ifndef IOPERATION_H
#define IOPERATION_H

#include <QString>

namespace reversehash {
	class IOperation
    {
        public:
			virtual QString type() = 0;
			virtual bool calc(bool b1, bool b2) = 0;
    };
}

#endif // IOPERATION_H
