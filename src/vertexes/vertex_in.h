#ifndef VERTEX_IN_H
#define VERTEX_IN_H

#include "../interfaces/ivertex_out.h"
#include "../interfaces/ivertex_in.h"
#include <QString>

namespace reversehash {
	class VertexIn : public virtual IVertexOut, virtual public IVertexIn
    {
        public:
			VertexIn();

			// IVertexOut
			virtual bool out();
			virtual QString type();
			virtual QString name();
			virtual void setName(QString sName);
			virtual void setXYZ(float x, float y, float z);
			virtual float x();
			virtual float y();
			virtual float z();
			
			// IVertexIn
            virtual void setNumber(int n);
            virtual int number();
			virtual void setValue(bool bValue);

		private:
            int m_nNumber;
			QString m_sName;
			bool m_bValue;
			float m_nX, m_nY, m_nZ;
    };
}

#endif // VERTEX_IN_H
