#ifndef SEA5KG_BNA_OPERATIONS_H
#define SEA5KG_BNA_OPERATIONS_H

#include "bna_types.h"

class IBNAOper {
    public:
        virtual std::string type() = 0;
        virtual BNABit calc(BNABit b1, BNABit b2) = 0;
};

class BNAOperXor : public IBNAOper{
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

class BNAOperNotXor : public IBNAOper{
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

class BNAOperAnd : public IBNAOper {
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

class BNAOperOr : public IBNAOper {
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

#endif // SEA5KG_BNA_OPERATIONS_H