#ifndef SEA5KG_BNA_TYPES_H
#define SEA5KG_BNA_TYPES_H

#include <vector>
#include <string>

enum BNABit : char {
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

template<class ValueType> class BNAVar {
    public:
        BNAVar() {
            // m_bVal = B_0;
            m_sName = "";
        }
        const std::string &getName() {
            return m_sName;
        }
        void setName(const std::string &sName) {
            m_sName = sName;
        }

        ValueType getValue() {
            return m_bVal;
        }
        void setValue(ValueType bVal) {
            m_bVal = bVal;
        }
    private:
        ValueType m_bVal;
        std::string m_sName;
};

#endif // SEA5KG_BNA_TYPES_H