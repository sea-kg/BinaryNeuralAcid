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
			~Vertex();
			
			// IVertexOut
			virtual bool out();
			virtual QString type();
			virtual QString name();
			virtual void setName(QString sName);
			virtual void setXYZ(float x, float y, float z);
			virtual float x();
			virtual float y();
			virtual float z();
			virtual void setX(float x);
			virtual void setY(float y);
			virtual void setZ(float z);
			
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
			float m_nX, m_nY, m_nZ;
    };
}

#endif // VERTEX_H
