#include "binary_neural_acid.h"

#include <vector>

int main() {
    const std::string sPath0 = "./char-test0";
    const std::string sPath1 = "./char-test1";
    BinaryNeuralAcidConfig config;
    config
        .setInputSize(1)
        .setNodesSize(1024)
        .setOutputSize(1)
    ;

    BinaryNeuralAcid<char> bna;
    bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.randomGenerate(config);
    bna.removeAllDeadlockNodes();
    bna.save(sPath0);
    bna.exportToDot(sPath0);

    int nMutationCicles = 10;
    int nAddCicles = 1000;
    int nRemoveCicles = 20;
    auto pModelModify = new BinaryNeuralAcidModificationModel(nMutationCicles, nAddCicles, nRemoveCicles);

    bool bStop = false;
    int nCicles = 0;
    while (!bStop) {
        nCicles++;
        bStop = true;
        bna.randomModify(pModelModify);
        bna.removeAllDeadlockNodes();

        bna.save(sPath1);
        bna.exportToDot(sPath1);
        int sum = 0;
        for (int i = 0; i < 256; i++) {
            char c0 = i;
            unsigned char c = bna.compute({c0}, 0);
            sum += (int)c;
        }
        if (sum == 0) {
            bStop = false;
        }
    }

    for (int i = 0; i < 256; i++) {
        char c0 = i;
        unsigned char c = bna.compute({c0}, 0);
        unsigned char cExpected = c0;
        // std::cout << (unsigned int)(unsigned char)c0 << " => " << (int)c << std::endl;
        if (c != cExpected) {
            std::cerr << i << ", expected: " << cExpected << ", but got " << c << std::endl;
            return 1;
        }
    }
    // std::cout << "nCicles = " << nCicles << std::endl;

    return 0;
}