#include "binary_neural_acid.h"

#include <vector>

int check_xor_result(BinaryNeuralAcid<BinaryNeuralAcidBit> &bna) {

    int nRet = 0;
    if (bna.compute({BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_0}, 0) != BinaryNeuralAcidBit::B_0) {
        nRet++;
    }

    if (bna.compute({BinaryNeuralAcidBit::B_0, BinaryNeuralAcidBit::B_1}, 0) != BinaryNeuralAcidBit::B_1) {
        nRet++;
    }

    if (bna.compute({BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_0}, 0) != BinaryNeuralAcidBit::B_1) {
        nRet++;
    }

    if (bna.compute({BinaryNeuralAcidBit::B_1, BinaryNeuralAcidBit::B_1}, 0) != BinaryNeuralAcidBit::B_0) {
        nRet++;
    }
    return nRet;
};

int main() {
    BinaryNeuralAcidConfig config;
    config
        .setInputSize(2)
        .setNodesSize(5)
        .setOutputSize(1)
    ;

    BinaryNeuralAcid<BinaryNeuralAcidBit> bna;
    bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.randomGenerate(config);

    int nMutationCicles = 10;
    int nAddCicles = 1024;
    int nRemoveCicles = 20;
    auto pModelModify = new BinaryNeuralAcidModificationModel(nMutationCicles, nAddCicles, nRemoveCicles);

    int res = check_xor_result(bna);
    int nSafeCicles = 0;
    while (res != 0) {
        nSafeCicles++;
        if (nSafeCicles > 100) {
            std::cout << "res " << res << std::endl;
            return 1;
        }
        bna.randomModify(pModelModify);
        bna.removeAllDeadlockNodes();
        res = check_xor_result(bna);
        if (res == 0) {
            std::cout << " Found on cicle: " << nSafeCicles << std::endl;
            bna.save("xor");
            bna.exportToDot("xor");
            bna.exportToCpp("xor");
            return 0;
        }
    }

    return 1;
}