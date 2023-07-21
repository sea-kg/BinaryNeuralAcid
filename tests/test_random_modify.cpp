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
    const std::string sPathBase = "./random-mutation";
    const std::string sPath0 = "./random-mutation0";
    const std::string sPath1 = "./random-mutation1";
    const std::string sPath2 = "./random-mutation2";

    BinaryNeuralAcidConfig config;
    config
        .setInputSize(8)
        .setNodesSize(1224)
        .setOutputSize(8)
    ;

    BinaryNeuralAcid bna;
    bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.randomGenerate(config);
    bna.removeAllDeadlockNodes();
    bna.exportToDot(sPath0);

    int nMutationCicles = 10;
    int nAddCicles = 100;
    int nRemoveCicles = 20;
    auto pModelModify = new BinaryNeuralAcidModificationModel(nMutationCicles, nAddCicles, nRemoveCicles);

    std::vector<BinaryNeuralAcidBit> vInputs;
    for (int i = 0; i < bna.getInputSize(); i++) {
        vInputs.push_back(B_1);
    }

    std::string sResult0 = test_calculate(bna, vInputs);
    for (int i = 0; i < 10; i++) {
        bna.exportToDot(sPathBase + std::to_string(i));

        bna.randomModify(pModelModify);
        bna.removeAllDeadlockNodes();
        std::string sResult1 = test_calculate(bna, vInputs);
        if (sResult0 == sResult1) {
            std::cerr << "cicle " << i << " ... result could be equals, sResult0: " << sResult0 << ", sResult1: " << sResult1 << std::endl;
            return 1;
        }
        sResult0 = sResult1;
    }

    return 0;
}