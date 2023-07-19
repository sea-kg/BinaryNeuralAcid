#include "binary_neural_acid.h"

#include <vector>
#include <wsjcpp_core.h>

int main() {
    WsjcppCore::makeDir("./temporary-unit-tests-data");

    BinaryNeuralAcidConfig config;
    config
        .setInputSize(128)
        .setNodesSize(256)
        .setOutputSize(1)
    ;

    BinaryNeuralAcid bna;
    bna.randomGenerate(config);

    // fill the input vector
    std::vector<BinaryNeuralAcidBit> vInputs;
    for (int i = 0; i < config.getInputSize(); i++) {
        vInputs.push_back(B_1);
    }
    BinaryNeuralAcidBit bResult = bna.calc(vInputs, 0);

    std::string sPath0 = "./temporary-unit-tests-data/read-write-test0";
    if (!bna.save(sPath0)) {
        std::cerr << "Could not save to file: " << sPath0 << std::endl;
        return 1;
    }

    if (!bna.load(sPath0)) {
        std::cerr << "Could not load from file: " << sPath0 << std::endl;
        return 1;
    }

    BinaryNeuralAcidBit bResultNew0 = bna.calc(vInputs, 0);
    if (bResult != bResultNew0) {
        std::cerr << "Different result for " << sPath0 << std::endl;
        return 1;
    }

    std::string sPath1 = "./temporary-unit-tests-data/read-write-test1";
    if (!bna.save(sPath1)) {
        std::cerr << "Could not save to file: " << sPath1 << std::endl;
        return 1;
    }

    if (!bna.load(sPath1)) {
        std::cerr << "Could not load from file: " << sPath1 << std::endl;
        return 1;
    }

    BinaryNeuralAcidBit bResultNew1 = bna.calc(vInputs, 0);
    if (bResult != bResultNew1) {
        std::cerr << "Different result for " << sPath1 << std::endl;
        return 1;
    }
    return 0;
}