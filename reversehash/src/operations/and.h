#ifndef OPERATION_AND_H
#define OPERATION_AND_H

#include "../interfaces/ioperation.h"
#include <QString>

namespace reversehash {
	class OperationAnd : public IOperation
    {
        public:
			virtual QString type();
			virtual bool calc(bool b1, bool b2);
    };
}

#endif // OPERATION_AND_H
