#include <string.h>
#include <iostream>
#include "binary_neural_acid.h"

int check_sum_result(BinaryNeuralAcid<char> &bna) {
    int nRet = 0;
    for (int  x = 0; x < 256; x++) {
        for (int  y = 0; y < 256; y++) {
            char cX = x;
            char cY = y;
            char expected = x + y;
            if (bna.compute({cX, cY}, 0) != expected) {
                nRet++;
            }
        }
    }
    return nRet;
}

int main(int argc, const char* argv[]) {

    BinaryNeuralAcidConfig config;
    config
        .setInputSize(2)
        .setNodesSize(5)
        .setOutputSize(1)
    ;

    BinaryNeuralAcid<char> bna;
    bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.randomGenerate(config);

    if (BinaryNeuralAcidHelpers::fileExists("sum.bna")) {
        bna.load("sum");
    }

    int nMutationCicles = 10;
    int nAddCicles = 1024;
    int nRemoveCicles = 20;

    // auto pModelModify = new BinaryNeuralAcidModificationModel(nMutationCicles, nAddCicles, nRemoveCicles);

    std::vector<BinaryNeuralAcidModificationModel *> m_vModificationModels;
    m_vModificationModels.push_back(new BinaryNeuralAcidModificationModel(rand() % 1000, rand() % 1000, rand() % 1000));
    m_vModificationModels.push_back(new BinaryNeuralAcidModificationModel(nMutationCicles, nAddCicles, nRemoveCicles));
    m_vModificationModels.push_back(new BinaryNeuralAcidModificationModel(1,0,0));
    m_vModificationModels.push_back(new BinaryNeuralAcidModificationModel(0,10,0));
    m_vModificationModels.push_back(new BinaryNeuralAcidModificationModel(0,0,1));

    int res = check_sum_result(bna);
    std::cout << " res: " << res << " on start " << std::endl;
    bna.save("sum");
    int nSafeCicles = 0;
    int nModModelNumber = 0;
    while (res != 0) {
        nSafeCicles++;
        nModModelNumber = (nModModelNumber + 1) % m_vModificationModels.size();
        if (nSafeCicles % 500 == 0) {
            std::cout << " (+500) res: " << res << " on iter: " << nSafeCicles << std::endl;
            m_vModificationModels[0]->update(rand() % 1000, rand() % 1000, rand() % 1000);
        }
        // if (nSafeCicles > 10000) {
        //     return 1;
        // }

        bna.randomModify(m_vModificationModels[nModModelNumber]);
        bna.removeAllDeadlockNodes();
        int next_res = check_sum_result(bna);
        if (next_res < res) {
            res = next_res;
            std::cout << " res: " << res << " on iter: " << nSafeCicles << std::endl;
            bna.save("sum");
            bna.exportToDot("sum");
        } else {
            bna.load("sum");
        }
        // std::cout << " res: " << res << std::endl;

        // if (res == 0) {
        //     std::cout << " Found on cicle: " << nSafeCicles << std::endl;
        //     bna.save("sum");
        //     bna.exportToDot("sum");
        //     // bna.exportToCpp("xor");
        //     return 0;
        // }
    }
    return 0;
}

