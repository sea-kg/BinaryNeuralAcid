#ifndef IVERTEX_IN_H
#define IVERTEX_IN_H

#include <QString>

namespace reversehash {
	class IVertexIn
    {
        public:
			virtual ~IVertexIn() {};
            virtual void setNumber(int n) = 0;
            virtual int number() = 0;
			virtual void setValue(bool bValue) = 0;
    };
}

#endif // IVERTEX_IN_H
