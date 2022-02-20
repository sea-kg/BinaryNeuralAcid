
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


// -----------------------------------------------------------------
// BNAVar just contains boolean variable

BNAVar::BNAVar() {
   m_bVal = B_0;
   m_sName = "";
}

void BNAVar::name(std::string name){
    m_sName = name;
}

std::string BNAVar::name(){
    return m_sName;
}

BNABit BNAVar::val(){
    return m_bVal;
}

void BNAVar::val(BNABit bVal){
    m_bVal = bVal;
}