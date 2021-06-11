#ifndef SEA5KG_BNA_TYPES_H
#define SEA5KG_BNA_TYPES_H

#include <vector>

enum BNABit{
    B_0 = 0x00,
    B_1 = 0x01
};

class BNABit4 {
    public:
        BNABit4(BNABit b1, BNABit b2, BNABit b3, BNABit b4);
        void appendToVector(std::vector<BNABit> &vars);
        BNABit b1;
        BNABit b2;
        BNABit b3;
        BNABit b4;
};

#endif // SEA5KG_BNA_TYPES_H