#ifndef OPERATION_OR_H
#define OPERATION_OR_H

#include "../interfaces/ioperation.h"
#include <QString>

namespace reversehash {
	class OperationOr : public IOperation
    {
        public:
			virtual QString type();
			virtual bool calc(bool b1, bool b2);
    };
}

#endif // OPERATION_OR_H
