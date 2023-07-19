#include "binary_neural_acid.h"

#include <vector>

int main() {

    // bit -> bin string
    if (BinaryNeuralAcidBitConvertor::toBinString(BinaryNeuralAcidBit::B_0) != "0") {
        std::cerr << "Expected 0" << std::endl;
        return 1;
    }
    if (BinaryNeuralAcidBitConvertor::toBinString(BinaryNeuralAcidBit::B_1) != "1") {
        std::cerr << "Expected 1" << std::endl;
        return 1;
    }

    if (BinaryNeuralAcidBitConvertor::toBinString((BinaryNeuralAcidBit)0x02) != "?") {
        std::cerr << "Expected ?" << std::endl;
        return 1;
    }

    // bit4 -> bin string
    {
        BinaryNeuralAcidBit4 bit4(BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1);
        if (BinaryNeuralAcidBitConvertor::toBinString(bit4) != "0001") {
            std::cerr << "Expected 0001" << std::endl;
            return 1;
        }
        for (int b1 = 0; b1 <= 1; b1++) {
            for (int b2 = 0; b2 <= 1; b2++) {
                for (int b3 = 0; b3 <= 1; b3++) {
                    for (int b4 = 0; b4 <= 1; b4++) {
                        bit4.b1 = (BinaryNeuralAcidBit)b1;
                        bit4.b2 = (BinaryNeuralAcidBit)b2;
                        bit4.b3 = (BinaryNeuralAcidBit)b3;
                        bit4.b4 = (BinaryNeuralAcidBit)b4;
                        std::string sExpected = std::to_string(b1) + std::to_string(b2) + std::to_string(b3) + std::to_string(b4);
                        std::string sGot = BinaryNeuralAcidBitConvertor::toBinString(bit4);
                        if (sGot != sExpected) {
                            std::cerr << "Expected " << sExpected << ", but got " << sGot << std::endl;
                            return 1;
                        }
                    }
                }
            }
        }
    }

    // to hex
    {
        BinaryNeuralAcidBit4 bit4(BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1);
        if (BinaryNeuralAcidBitConvertor::toHexString(bit4) != "d") {
            std::cerr << "Expected 'd'" << std::endl;
            return 1;
        }
        const std::string sExpected = "0123456789abcdef";
        std::string sGot = "";
        for (int b1 = 0; b1 <= 1; b1++) {
            for (int b2 = 0; b2 <= 1; b2++) {
                for (int b3 = 0; b3 <= 1; b3++) {
                    for (int b4 = 0; b4 <= 1; b4++) {
                        bit4.b1 = (BinaryNeuralAcidBit)b1;
                        bit4.b2 = (BinaryNeuralAcidBit)b2;
                        bit4.b3 = (BinaryNeuralAcidBit)b3;
                        bit4.b4 = (BinaryNeuralAcidBit)b4;
                        // std::cout << BinaryNeuralAcidBitConvertor::toString(bit4) << std::endl;
                        sGot += BinaryNeuralAcidBitConvertor::toHexString(bit4);
                    }
                }
            }
        }
        if (sGot != sExpected) {
            std::cerr << "Expected " << sExpected << ", but got " << sGot << std::endl;
            return 1;
        }
    }

    // hex to bin
    {
        std::vector<std::pair<std::string, std::string>> vTests;
        vTests.push_back(std::pair<std::string, std::string>("", ""));
        vTests.push_back(std::pair<std::string, std::string>("0", "0000"));
        vTests.push_back(std::pair<std::string, std::string>("1", "0001"));
        vTests.push_back(std::pair<std::string, std::string>("2", "0010"));
        vTests.push_back(std::pair<std::string, std::string>("3", "0011"));
        vTests.push_back(std::pair<std::string, std::string>("4", "0100"));
        vTests.push_back(std::pair<std::string, std::string>("5", "0101"));
        vTests.push_back(std::pair<std::string, std::string>("6", "0110"));
        vTests.push_back(std::pair<std::string, std::string>("7", "0111"));
        vTests.push_back(std::pair<std::string, std::string>("8", "1000"));
        vTests.push_back(std::pair<std::string, std::string>("9", "1001"));
        vTests.push_back(std::pair<std::string, std::string>("A", "1010"));
        vTests.push_back(std::pair<std::string, std::string>("B", "1011"));
        vTests.push_back(std::pair<std::string, std::string>("C", "1100"));
        vTests.push_back(std::pair<std::string, std::string>("D", "1101"));
        vTests.push_back(std::pair<std::string, std::string>("E", "1110"));
        vTests.push_back(std::pair<std::string, std::string>("F", "1111"));
        vTests.push_back(std::pair<std::string, std::string>("a", "1010"));
        vTests.push_back(std::pair<std::string, std::string>("b", "1011"));
        vTests.push_back(std::pair<std::string, std::string>("c", "1100"));
        vTests.push_back(std::pair<std::string, std::string>("d", "1101"));
        vTests.push_back(std::pair<std::string, std::string>("e", "1110"));
        vTests.push_back(std::pair<std::string, std::string>("f", "1111"));
        vTests.push_back(std::pair<std::string, std::string>("01", "00000001"));
        vTests.push_back(std::pair<std::string, std::string>("9f", "10011111"));
        vTests.push_back(std::pair<std::string, std::string>("9fgg", "10011111????????"));

        for (int i = 0; i < vTests.size(); i++) {
            std::string sExpected = vTests[i].second;
            std::string sGot = BinaryNeuralAcidBitConvertor::toBinStringFromHexString(vTests[i].first);
            if (sExpected != sGot) {
                std::cerr << "BinaryNeuralAcidBitConvertor::toBinStringFromHexString: Expected '" << sExpected << "', but got '" << sGot << "' ... for " << vTests[i].first << std::endl;
                return 1;
            }
        }
    }

    // toBinStringFromBits
    {
        std::vector<BinaryNeuralAcidBit> bits = {B_0,B_1,B_1,B_0,B_1};
        std::string sExpected = "01101";
        std::string sGot = BinaryNeuralAcidBitConvertor::toBinStringFromBits(bits);
        if (sExpected != sGot) {
            std::cerr << "BinaryNeuralAcidBitConvertor::toBinStringFromBits: Expected '" << sExpected << "', but got '" << sGot << std::endl;
            return 1;
        }
    }

    // bin to hex
    {
        std::vector<std::pair<std::string, std::string>> vTests;
        vTests.push_back(std::pair<std::string, std::string>("", ""));
        vTests.push_back(std::pair<std::string, std::string>("a000", "?"));
        vTests.push_back(std::pair<std::string, std::string>("000s", "?"));
        vTests.push_back(std::pair<std::string, std::string>("a0001111", "?f"));
        vTests.push_back(std::pair<std::string, std::string>("0s001111", "?f"));
        vTests.push_back(std::pair<std::string, std::string>("00s01111", "?f"));
        vTests.push_back(std::pair<std::string, std::string>("000s1111", "?f"));
        vTests.push_back(std::pair<std::string, std::string>("0000", "0"));
        vTests.push_back(std::pair<std::string, std::string>("0001", "1"));
        vTests.push_back(std::pair<std::string, std::string>("0010", "2"));
        vTests.push_back(std::pair<std::string, std::string>("0011", "3"));
        vTests.push_back(std::pair<std::string, std::string>("0100", "4"));
        vTests.push_back(std::pair<std::string, std::string>("0101", "5"));
        vTests.push_back(std::pair<std::string, std::string>("0110", "6"));
        vTests.push_back(std::pair<std::string, std::string>("0111", "7"));
        vTests.push_back(std::pair<std::string, std::string>("1000", "8"));
        vTests.push_back(std::pair<std::string, std::string>("1001", "9"));
        vTests.push_back(std::pair<std::string, std::string>("1010", "a"));
        vTests.push_back(std::pair<std::string, std::string>("1011", "b"));
        vTests.push_back(std::pair<std::string, std::string>("1100", "c"));
        vTests.push_back(std::pair<std::string, std::string>("1101", "d"));
        vTests.push_back(std::pair<std::string, std::string>("1110", "e"));
        vTests.push_back(std::pair<std::string, std::string>("1111", "f"));
        vTests.push_back(std::pair<std::string, std::string>("00000001", "01"));
        vTests.push_back(std::pair<std::string, std::string>("10011111", "9f"));
        vTests.push_back(std::pair<std::string, std::string>("10011111000", "9f"));
        vTests.push_back(std::pair<std::string, std::string>("10011111somesome1000", "9f??8"));
        vTests.push_back(std::pair<std::string, std::string>("10011111ss", "9f"));

        for (int i = 0; i < vTests.size(); i++) {
            std::string sExpected = vTests[i].second;
            std::string sGot = BinaryNeuralAcidBitConvertor::toHexStringFromBinString(vTests[i].first);
            if (sExpected != sGot) {
                std::cerr << "BinaryNeuralAcidBitConvertor::toHexStringFromBinString: Expected '" << sExpected << "', but got '" << sGot << "' ... for " << vTests[i].first << std::endl;
                return 1;
            }
        }
    }

    // toBitsFromBinString
    {
        std::vector<BinaryNeuralAcidBit> bits = BinaryNeuralAcidBitConvertor::toBitsFromBinString("1011");
        if (bits.size() != 4) {
            std::cerr << "BinaryNeuralAcidBitConvertor::toBitsFromBinString: Expected 4 bits " << std::endl;
            return 1;
        }

        if (bits[0] != B_1 || bits[1] != B_0 || bits[2] != B_1 || bits[3] != B_1) {
            std::cerr << "BinaryNeuralAcidBitConvertor::toBitsFromBinString: Unexpected bits " << std::endl;
            return 1;
        }
    }
    // toBitsFromHexString
    {
        std::vector<BinaryNeuralAcidBit> bits = BinaryNeuralAcidBitConvertor::toBitsFromHexString("9f");
        if (bits.size() != 8) {
            std::cerr << "BinaryNeuralAcidBitConvertor::toBitsFromHexString: Expected 8 bits " << std::endl;
            return 1;
        }
        // 9f -> 10011111
        if (
            bits[0] != B_1 || bits[1] != B_0 || bits[2] != B_0 || bits[3] != B_1
            || bits[4] != B_1 || bits[5] != B_1 || bits[6] != B_1 || bits[7] != B_1
        ) {
            // 9f
            std::cerr << "BinaryNeuralAcidBitConvertor::toBitsFromHexString: Unexpected bits " << std::endl;
            return 1;
        }
    }

    // toHexStringFromBits
    {
        std::vector<BinaryNeuralAcidBit> bits = {B_0,B_1,B_1,B_1,B_1}; // 01111
        std::string sExpected = "7"; // 0111
        std::string sGot = BinaryNeuralAcidBitConvertor::toHexStringFromBits(bits);
        if (sExpected != sGot) {
            std::cerr << "BinaryNeuralAcidBitConvertor::toHexStringFromBits: Expected '" << sExpected << "', but got '" << sGot << std::endl;
            return 1;
        }
    }

    return 0;
}