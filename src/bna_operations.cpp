
#include "bna_operations.h"

// -----------------------------------------------------------------
// BNAOperXor

std::string BNAOperXor::type(){ return std::string("XOR"); }

BNABit BNAOperXor::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 ^ c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------
// BNAOperNotXor

std::string BNAOperNotXor::type(){ return std::string("NXOR"); }

BNABit BNAOperNotXor::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (!(c1 ^ c2)) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------
// BNAOperAnd

std::string BNAOperAnd::type(){ return std::string("AND"); }

BNABit BNAOperAnd::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 & c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}

// -----------------------------------------------------------------
// BNAOperOr

std::string BNAOperOr::type(){ return std::string("OR"); }

BNABit BNAOperOr::calc(BNABit b1, BNABit b2){
    unsigned char c1 = b1;
    unsigned char c2 = b2;
    c1 = (c1 | c2) & 0x01;
    return c1 == 0 ? B_0 : B_1;
}