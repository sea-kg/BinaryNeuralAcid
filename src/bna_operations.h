#ifndef SEA5KG_BNA_OPERATIONS_H
#define SEA5KG_BNA_OPERATIONS_H

#include "bna_types.h"

class IBNAOper {
    public:
        virtual std::string type() = 0;
        virtual BNABit calc(BNABit b1, BNABit b2) = 0;
};

// white
class BNAOperXor : public IBNAOper{
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

// yellow
class BNAOperNotXor : public IBNAOper{
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

// red
class BNAOperAnd : public IBNAOper {
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

// blue
class BNAOperOr : public IBNAOper {
    public:
        virtual std::string type();
        virtual BNABit calc(BNABit b1, BNABit b2);
};

#endif // SEA5KG_BNA_OPERATIONS_H