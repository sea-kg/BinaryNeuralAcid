#include "binary_neural_acid.h"

#include <vector>
#include <iostream>


int main() {
    IBNAOper<BinaryNeuralAcidBit> *pOper = new BNAOperXor();

    if (pOper->type() != "XOR") {
        return 1;
    }

    if (pOper->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }

    if (pOper->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }

    if (pOper->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }

    if (pOper->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }

    return 0;
}
