#include "binary_neural_acid.h"

#include <vector>
#include <iostream>


int main() {
    {
        IBinaryNeuralAcidOperation<unsigned char> *pOperAnd = new BinaryNeuralAcidOperationCharAnd();
        if (pOperAnd->type() != "AND") {
            return 1;
        }
        if (pOperAnd->calc(0x01, 0x01) != 0x01) {
            return 1;
        }
        if (pOperAnd->calc(0x01, 0x02) != 0x0) {
            return 1;
        }
    }

    {
        IBinaryNeuralAcidOperation<unsigned char> *pOperOr = new BinaryNeuralAcidOperationCharOr();
        if (pOperOr->type() != "OR") {
            return 1;
        }
        if (pOperOr->calc(0x01, 0x01) != 0x01) {
            return 1;
        }
        if (pOperOr->calc(0x01, 0x02) != 0x03) {
            return 1;
        }
    }

    {
        IBinaryNeuralAcidOperation<unsigned char> *pOperXor = new BinaryNeuralAcidOperationCharXor();
        if (pOperXor->type() != "XOR") {
            return 1;
        }
        if (pOperXor->calc(0x01, 0x01) != 0x00) {
            std::cerr << "xor 1 1 " << std::endl;
            return 1;
        }
        if (pOperXor->calc(0x01, 0x00) != 0x01) {
            std::cerr << "xor 1 0 " << std::endl;
            return 1;
        }
        if (pOperXor->calc(0x01, 0x02) != 0x03) {
            return 1;
        }
    }

    {
        IBinaryNeuralAcidOperation<unsigned char> *pOperShiftLeft = new BinaryNeuralAcidOperationCharShiftLeft();
        if (pOperShiftLeft->type() != "SHL") {
            return 1;
        }
        char c = pOperShiftLeft->calc(0x01, 0x04);
        if (c != 0x10) {
            std::cerr << "SHL 1 4 " << std::endl;
            return 1;
        }
        c = pOperShiftLeft->calc(0x01, 0x01);
        if (c != 0x02) {
            std::cerr << "SHL 1 1 " << std::endl;
            return 1;
        }
        c = pOperShiftLeft->calc(0x01, 0x07);
        if (c != char(0x80)) { // 1000 0000
            std::cerr << "SHL 1 7 " << (unsigned int)(unsigned char)c << std::endl;
            return 1;
        }
        c = pOperShiftLeft->calc(0x01, 0x08);
        if (c != char(0x01)) { // 0000 0001
            std::cerr << "SHL 1 8 " << (unsigned int)(unsigned char)c << std::endl;
            return 1;
        }
    }

    {
        IBinaryNeuralAcidOperation<unsigned char> *pOperShiftRight = new BinaryNeuralAcidOperationCharShiftRight();
        if (pOperShiftRight->type() != "SHR") {
            return 1;
        }
        char c = pOperShiftRight->calc(0x01, 0x04);
        if (c != 0x10) {
            std::cerr << "SHR 1 4 " << std::endl;
            return 1;
        }
        c = pOperShiftRight->calc(0x01, 0x01);
        if (c != char(0x80)) {
            std::cerr << "SHR 1 1 " << std::endl;
            return 1;
        }
        c = pOperShiftRight->calc(0x01, 0x07);
        if (c != char(0x02)) { // 0000 0010
            std::cerr << "SHR 1 7 " << (unsigned int)(unsigned char)c << std::endl;
            return 1;
        }
        c = pOperShiftRight->calc(0x01, 0x08);
        if (c != char(0x01)) { // 0000 0001
            std::cerr << "SHR 1 8 " << (unsigned int)(unsigned char)c << std::endl;
            return 1;
        }
    }

}