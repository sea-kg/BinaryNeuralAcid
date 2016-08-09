#ifndef IVERTEX_OUT_H
#define IVERTEX_OUT_H

#include <QString>

namespace reversehash {
	class IVertexOut
    {
        public:
			virtual bool out() = 0;
			virtual QString type() = 0;
			virtual QString name() = 0;
			virtual void setName(QString sName) = 0;
			virtual void setXYZ(float x, float y, float z) = 0;
			virtual float x() = 0;
			virtual float y() = 0;
			virtual float z() = 0;
			virtual void setX(float x) = 0;
			virtual void setY(float y) = 0;
			virtual void setZ(float z) = 0;
    };
}

#endif // IVERTEX_OUT_H
