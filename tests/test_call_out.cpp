#include "binary_neural_acid.h"

#include <vector>
#include <iostream>


int main() {
    BinaryNeuralAcid<BinaryNeuralAcidBit> bna(3,1);

    int nodeN1 = bna.addNode(0, 1, "AND");
    if (nodeN1 != 3) {
        std::cerr << "nodeN1 != 3, got " << nodeN1 << std::endl;
        return 1;
    }
    int nodeN2 = bna.addNode(1, 2, "OR");
    if (nodeN2 != 4) {
        std::cerr << "nodeN2 != 4" << std::endl;
        return 1;
    }
    int nodeN3 = bna.addNode(nodeN1, nodeN2, "AND");
    if (nodeN3 != 5) {
        std::cerr << "nodeN3 != 5" << std::endl;
        return 1;
    }
    int outputNodeId = bna.addNodeOutput(nodeN3);
    if (outputNodeId != 0) {
        std::cerr << "outputNodeId != 0" << std::endl;
        return 1;
    }

    struct LocalDataTest {
        LocalDataTest(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2, BinaryNeuralAcidBit b3, BinaryNeuralAcidBit bExpected) {
            m_b1 = b1;
            m_b2 = b2;
            m_b3 = b3;
            m_bExpected = bExpected;
        }
        BinaryNeuralAcidBit m_b1;
        BinaryNeuralAcidBit m_b2;
        BinaryNeuralAcidBit m_b3;
        BinaryNeuralAcidBit m_bExpected;

        int check(BinaryNeuralAcid<BinaryNeuralAcidBit> &bna, std::string sCheckName) {
            BinaryNeuralAcidBit result = bna.compute({m_b1, m_b2, m_b3}, 0);
            if (result != m_bExpected) {
                std::cerr
                    << "failed: callout "
                    << std::to_string(int(m_b1)) << "_" << std::to_string(int(m_b2)) << "_" << std::to_string(int(m_b3))
                    << std::endl
                    << "   Expected " << BinaryNeuralAcidBitConvertor::toBinString(m_bExpected)
                    << ", but got: " << BinaryNeuralAcidBitConvertor::toBinString(result)
                    << std::endl;
                return 1;
            }
            return 0;
        }
    };

    std::vector<LocalDataTest> vTests;

    vTests.push_back(LocalDataTest(B_0, B_0, B_0, B_0));
    vTests.push_back(LocalDataTest(B_0, B_0, B_1, B_0));
    vTests.push_back(LocalDataTest(B_0, B_1, B_0, B_0));
    vTests.push_back(LocalDataTest(B_0, B_1, B_1, B_0));
    vTests.push_back(LocalDataTest(B_1, B_0, B_0, B_0));
    vTests.push_back(LocalDataTest(B_1, B_0, B_1, B_0));
    vTests.push_back(LocalDataTest(B_1, B_1, B_0, B_1));
    vTests.push_back(LocalDataTest(B_1, B_1, B_1, B_1));


    // bna.compile();

    for (int i = 0; i < vTests.size(); i++) {
        if (vTests[i].check(bna, "callout_first") != 0) {
            return 1;
        }
    }

    // save
    if (bna.save("./callout-test0") != true) {
        std::cerr << "failed: save (1) callout-test0" << std::endl;
        return 1;
    }

    // callout_after_save
    for (int i = 0; i < vTests.size(); i++) {
        if (vTests[i].check(bna, "callout_after_save") != 0) {
            return 1;
        }
    }

    // load from file
    if (!bna.load("./callout-test0")) {
        std::cerr << "failed: load_bna (reload)" << std::endl;
        return 1;
    }

    // callout_after_load
    for (int i = 0; i < vTests.size(); i++) {
        if (vTests[i].check(bna, "callout_after_load") != 0) {
            return 1;
        }
    }

    BinaryNeuralAcid<BinaryNeuralAcidBit> bna2(2,4);
    if (!bna2.load("./callout-test0")) {
        std::cerr << "failed: load_bna2" << std::endl;
        return 1;
    }

    // callout_after_load_bna2
    for (int i = 0; i < vTests.size(); i++) {
        if (vTests[i].check(bna2, "callout_after_load_bna2") != 0) {
            return 1;
        }
    }

    BinaryNeuralAcid<BinaryNeuralAcidBit> bna3;
    if (!bna3.load("./callout-test0")) {
        std::cerr << "failed: load_bna3" << std::endl;
        return 1;
    }

    // callout_after_load_bna3
    for (int i = 0; i < vTests.size(); i++) {
        if (vTests[i].check(bna3, "callout_after_load_bna3") != 0) {
            return 1;
        }
    }

    if (!bna.save("./callout-test1")) {
        std::cerr << "failed: save (1) callout-test0" << std::endl;
        return 1;
    }
    // compare("save1", bSave1, true);

    return 0;
}
