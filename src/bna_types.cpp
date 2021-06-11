
#include "bna_types.h"



// -----------------------------------------------------------------
// BNABit4

BNABit4::BNABit4(BNABit b1, BNABit b2, BNABit b3, BNABit b4) : b1(b1), b2(b2), b3(b3), b4(b4) {

}

void BNABit4::appendToVector(std::vector<BNABit> &vars){
    vars.push_back(b1);
    vars.push_back(b2);
    vars.push_back(b3);
    vars.push_back(b4);
}