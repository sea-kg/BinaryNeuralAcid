#ifndef VERTEX_H
#define VERTEX_H

#include "../interfaces/ivertex_out.h"
#include "../interfaces/ivertex_operation.h"
#include "../interfaces/ioperation.h"
#include <QString>

namespace reversehash {
	class Vertex : public IVertexOut, public IVertexOperation
    {
        public:
			Vertex();
			
			// IVertexOut
			virtual bool out();
			virtual QString type();
			virtual QString name();
			virtual void setName(QString sName);
			
			// IVertexOperation
			virtual void setOperation(QString sOper);
			virtual QString operation();
			virtual void setIn1(IVertexOut *pIn);
			virtual void setIn2(IVertexOut *pIn);
			virtual IVertexOut *in1();
			virtual IVertexOut *in2();

		private:
			IVertexOut *m_pIn1;
			IVertexOut *m_pIn2;
			IOperation *m_pOperation;
			QString m_sName;
    };
}

#endif // VERTEX_H
