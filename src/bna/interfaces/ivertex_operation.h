#ifndef IVERTEX_OPERATION_H
#define IVERTEX_OPERATION_H

#include "ivertex_out.h"

#include <QString>

namespace reversehash {
	class IVertexOperation
    {
        public:
			virtual ~IVertexOperation() {};
			virtual void setOperation(QString sOper) = 0;
			virtual QString operation() = 0;
			virtual void setIn1(IVertexOut *pInput) = 0;
			virtual void setIn2(IVertexOut *pInput) = 0;
			virtual IVertexOut *in1() = 0;
			virtual IVertexOut *in2() = 0;
    };
}

#endif // IVERTEX_OPERATION_H
