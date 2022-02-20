#ifndef SEA5KG_BNA_OPERATIONS_H
#define SEA5KG_BNA_OPERATIONS_H

#include "bna_types.h"

template<class ValueType> class IBNAOper {
    public:
        virtual std::string type() = 0;
        virtual ValueType calc(ValueType b1, ValueType b2) = 0;
};

class BNAOperXor : public IBNAOper<BNABit>{
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

class BNAOperNotXor : public IBNAOper<BNABit>{
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

class BNAOperAnd : public IBNAOper<BNABit> {
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

class BNAOperOr : public IBNAOper<BNABit> {
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

#endif // SEA5KG_BNA_OPERATIONS_H