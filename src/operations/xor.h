#ifndef OPERATION_XOR_H
#define OPERATION_XOR_H

#include "../interfaces/ioperation.h"
#include <QString>

namespace reversehash {
	class OperationXor : public IOperation
    {
        public:
			virtual QString type();
			virtual bool calc(bool b1, bool b2);
    };
}

#endif // OPERATION_XOR_H
