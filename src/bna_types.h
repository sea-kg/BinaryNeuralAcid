#ifndef SEA5KG_BNA_TYPES_H
#define SEA5KG_BNA_TYPES_H

#include <vector>
#include <string>

enum BNABit {
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

class BNAVar {
    public:
        BNAVar();
        std::string name();
        void name(std::string name);
        BNABit val();
        void val(BNABit bVal);
    private:
        BNABit m_bVal;
        std::string m_sName;
};

#endif // SEA5KG_BNA_TYPES_H