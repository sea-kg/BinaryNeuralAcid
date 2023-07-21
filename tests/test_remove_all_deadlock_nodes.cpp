#include "binary_neural_acid.h"

#include <vector>


std::string test_calculate(BinaryNeuralAcid<BinaryNeuralAcidBit> &bna, std::vector<BinaryNeuralAcidBit> &vInputs) {
    std::vector<BinaryNeuralAcidBit> vOutputs;
    for (int i = 0; i < bna.getOutputSize(); i++) {
        vOutputs.push_back(bna.compute(vInputs, i));
    }
    std::string sResult = BinaryNeuralAcidBitConvertor::toBinStringFromBits(vOutputs);
    return sResult;
}

int main() {
    const std::string sPath0 = "./remove-all-deadlock-nodes0";
    const std::string sPath1 = "./remove-all-deadlock-nodes1";

    BinaryNeuralAcidConfig config;
    config
        .setInputSize(8)
        .setNodesSize(1224)
        .setOutputSize(8)
    ;

    BinaryNeuralAcid<BinaryNeuralAcidBit> bna;
    bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.randomGenerate(config);

    bna.exportToDot(sPath0);

    // fill the input vector
    std::vector<BinaryNeuralAcidBit> vInputs;
    for (int i = 0; i < bna.getInputSize(); i++) {
        vInputs.push_back(B_1);
    }
    std::string sOutputsExpected = test_calculate(bna, vInputs);
    int nNodesSizeBefore = bna.getNodesSize();
    bna.removeAllDeadlockNodes();
    int nNodesSizeAfter = bna.getNodesSize();
    bna.exportToDot(sPath1);

    std::string sOutputsGot = test_calculate(bna, vInputs);
    if (sOutputsExpected != sOutputsGot) {
        std::cerr << "Different result after removeAllDeadlockNodes" << std::endl;
        std::cerr
            << "Expected " << sOutputsExpected
            << ", but got: " << sOutputsGot
            << std::endl;
        return 1;
    }

    if (nNodesSizeBefore == nNodesSizeAfter) {
        std::cerr << "No removed all nodes, before " << nNodesSizeBefore << ", after: " << nNodesSizeAfter << std::endl;
        return 1;
    }

    bna.removeAllDeadlockNodes();
    int nNodesSizeAfter0 = bna.getNodesSize();
    sOutputsGot = test_calculate(bna, vInputs);
    if (sOutputsExpected != sOutputsGot) {
        std::cerr << "(after0) Different result after removeAllDeadlockNodes" << std::endl;
        std::cerr
            << "Expected " << sOutputsExpected
            << ", but got: " << sOutputsGot
            << std::endl;
        return 1;
    }

    if (nNodesSizeAfter0 != nNodesSizeAfter) {
        std::cerr << "(after0) No removed all nodes, before " << nNodesSizeAfter0 << ", after: " << nNodesSizeAfter << std::endl;
        return 1;
    }

    return 0;
}