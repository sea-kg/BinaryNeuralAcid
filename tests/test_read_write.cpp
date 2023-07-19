#include "binary_neural_acid.h"

#include <vector>

int main() {
    const std::string sPath0 = "./read-write-test0";
    const std::string sPath1 = "./read-write-test1";
    const std::string sPath2 = "./read-write-test2";
    BinaryNeuralAcidConfig config;
    config
        .setInputSize(4)
        .setNodesSize(100)
        .setOutputSize(4)
    ;

    BinaryNeuralAcid bna;
    bna.randomGenerate(config);

    // fill the input vector
    std::vector<BinaryNeuralAcidBit> vInputs;
    for (int i = 0; i < config.getInputSize(); i++) {
        vInputs.push_back(B_1);
    }
    std::vector<BinaryNeuralAcidBit> vOutputs;
    for (int i = 0; i < config.getOutputSize(); i++) {
        vOutputs.push_back(bna.calc(vInputs, i));
    }
    std::string sOutputsExpected = BinaryNeuralAcidBitConvertor::toBinStringFromBits(vOutputs);

    if (!bna.save(sPath0)) {
        std::cerr << "Could not save to file: " << sPath0 << std::endl;
        return 1;
    }
    bna.exportToDot(sPath0);

    if (!bna.load(sPath0)) {
        std::cerr << "Could not load from file: " << sPath0 << std::endl;
        return 1;
    }
    bna.save(sPath1);
    bna.exportToDot(sPath1);

    bna.load(sPath1);
    bna.save(sPath2);
    bna.exportToDot(sPath2);

    std::vector<BinaryNeuralAcidBit> vOutputs0;
    for (int i = 0; i < config.getOutputSize(); i++) {
        vOutputs0.push_back(bna.calc(vInputs, i));
    }
    std::string sOutputsGot0 = BinaryNeuralAcidBitConvertor::toBinStringFromBits(vOutputs0);

    if (sOutputsExpected != sOutputsGot0) {
        std::cerr << "Different result for " << sPath0 << std::endl;
        std::cerr
            << "(path0) Expected " << sOutputsExpected
            << ", but got: " << sOutputsGot0
            << std::endl;
        return 1;
    }

    if (!bna.save(sPath1)) {
        std::cerr << "Could not save to file: " << sPath1 << std::endl;
        return 1;
    }

    if (!bna.load(sPath1)) {
        std::cerr << "Could not load from file: " << sPath1 << std::endl;
        return 1;
    }

    std::vector<BinaryNeuralAcidBit> vOutputs1;
    for (int i = 0; i < config.getOutputSize(); i++) {
        vOutputs1.push_back(bna.calc(vInputs, i));
    }
    std::string sOutputsGot1 = BinaryNeuralAcidBitConvertor::toBinStringFromBits(vOutputs1);

    if (sOutputsExpected != sOutputsGot1) {
        std::cerr << "(path1) Different result for " << sPath1 << std::endl;
        std::cerr
            << "Expected " << sOutputsExpected
            << ", but got: " << sOutputsGot1
            << std::endl;
        return 1;
    }
    return 0;
}