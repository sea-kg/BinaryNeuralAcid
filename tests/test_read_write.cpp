#include "binary_neural_acid.h"

#include <vector>


std::string test_calculate(BinaryNeuralAcid &bna, std::vector<BinaryNeuralAcidBit> &vInputs) {
    std::vector<BinaryNeuralAcidBit> vOutputs;
    for (int i = 0; i < bna.getOutputSize(); i++) {
        vOutputs.push_back(bna.calc(vInputs, i));
    }
    std::string sResult = BinaryNeuralAcidBitConvertor::toBinStringFromBits(vOutputs);
    return sResult;
}

int main() {
    const std::string sPath0 = "./read-write-test0";
    const std::string sPath1 = "./read-write-test1";
    const std::string sPath2 = "./read-write-test2";
    BinaryNeuralAcidConfig config;
    config
        .setInputSize(4)
        .setNodesSize(257)
        .setOutputSize(4)
    ;

    BinaryNeuralAcid bna;
    bna.randomGenerate(config);

    // fill the input vector
    std::vector<BinaryNeuralAcidBit> vInputs;
    for (int i = 0; i < config.getInputSize(); i++) {
        vInputs.push_back(B_1);
    }
    std::string sOutputsExpected = test_calculate(bna, vInputs);
    // std::cout << "sOutputsExpected: " << sOutputsExpected << std::endl;
    // std::cout << "1 test_calculate(bna, vInputs): " << test_calculate(bna, vInputs) << std::endl;
    // std::cout << "2 test_calculate(bna, vInputs): " << test_calculate(bna, vInputs) << std::endl;
    if (!bna.save(sPath0)) {
        std::cerr << "Could not save to file: " << sPath0 << std::endl;
        return 1;
    }
    // std::cout << "after save (1)... test_calculate(bna, vInputs): " << test_calculate(bna, vInputs) << std::endl;
    bna.exportToDot(sPath0);

    if (!bna.load(sPath0)) {
        std::cerr << "Could not load from file: " << sPath0 << std::endl;
        return 1;
    }
    // std::cout << "after load (1)... test_calculate(bna, vInputs): " << test_calculate(bna, vInputs) << std::endl;
    // bna.save(sPath1);
    // std::cout << "after save (2)... test_calculate(bna, vInputs): " << test_calculate(bna, vInputs) << std::endl;
    // bna.exportToDot(sPath1);

    std::string sOutputsGot0 = test_calculate(bna, vInputs);

    // bna.load(sPath1);
    // bna.save(sPath2);
    // bna.exportToDot(sPath2);

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
    std::string sOutputsGot1 = test_calculate(bna, vInputs);

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