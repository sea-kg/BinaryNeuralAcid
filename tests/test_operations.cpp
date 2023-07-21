#include "binary_neural_acid.h"

#include <vector>
#include <iostream>


int main() {
    IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *pOperAnd = new BinaryNeuralAcidOperationBitAnd();
    if (pOperAnd->type() != "AND") {
        return 1;
    }
    if (pOperAnd->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }
    if (pOperAnd->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }
    if (pOperAnd->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }
    if (pOperAnd->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }

    IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *pOperOr = new BinaryNeuralAcidOperationBitOr();
    if (pOperOr->type() != "OR") {
        return 1;
    }
    if (pOperOr->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }
    if (pOperOr->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }
    if (pOperOr->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }
    if (pOperOr->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }

    IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *pOperNotXor = new BinaryNeuralAcidOperationBitNotXor();
    if (pOperNotXor->type() != "NXOR") {
        return 1;
    }
    if (pOperNotXor->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }
    if (pOperNotXor->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }
    if (pOperNotXor->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }
    if (pOperNotXor->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }

    IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *pOperXor = new BinaryNeuralAcidOperationBitXor();
    if (pOperXor->type() != "XOR") {
        return 1;
    }
    if (pOperXor->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }
    if (pOperXor->calc(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }
    if (pOperXor->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0) != BinaryNeuralAcidBit::B_1) {
        return 1;
    }
    if (pOperXor->calc(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1) != BinaryNeuralAcidBit::B_0) {
        return 1;
    }

    return 0;
}