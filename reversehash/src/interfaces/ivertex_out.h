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
    };
}

#endif // IVERTEX_OUT_H
