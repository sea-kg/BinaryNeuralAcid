#include <string.h>
#include <iostream>
#include "binary_neural_acid.h"

struct ResultCheckSum {
    int nAll;
    int nSuccess;
    int nSuccessPercents;
};

void check_sum_result(BinaryNeuralAcid<unsigned char> &bna, ResultCheckSum &ret) {
    ret.nAll = 0;
    ret.nSuccess = 0;
    ret.nSuccessPercents = 0;
    int nRet = 0;
    for (int  x = 0; x < 256; x++) {
        for (int  y = 0; y < 256; y++) {
            unsigned char cX = x;
            unsigned char cY = y;
            unsigned char expected = x + y;
            if (bna.compute({cX, cY}, 0) == expected) {
                ret.nSuccess++;
            }/* else {
                std::cout << "cX = " << int(cX) << "; cY = " << int(cY) << ";" << std::endl;
            }*/
            ret.nAll++;
        }
    }
    ret.nSuccessPercents = (ret.nSuccess * 100) / ret.nAll;
}

int main(int argc, const char* argv[]) {

    BinaryNeuralAcidConfig config;
    config
        .setInputSize(2)
        .setNodesSize(5)
        .setOutputSize(1)
    ;

    BinaryNeuralAcid<unsigned char> bna;
    bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.randomGenerate(config);

    if (BinaryNeuralAcidHelpers::fileExists("example_sum.bna")) {
        bna.load("example_sum");
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

    ResultCheckSum res;
    ResultCheckSum nextResult;
    check_sum_result(bna, res);
    std::cout << " res: " << res.nSuccess << " / " << res.nAll << " (" << res.nSuccessPercents << "%) on start " << std::endl;
    bna.save("example_sum");
    bna.exportToCpp("example_sum");
    bna.exportToJavaScript("example_sum");
    int nSafeCicles = 0;
    int nModModelNumber = 0;
    while (res.nSuccess != 0) {
        nSafeCicles++;
        nModModelNumber = (nModModelNumber + 1) % m_vModificationModels.size();
        if (nSafeCicles % 500 == 0) {
            std::cout << " (+500) res: " << res.nSuccess << " / " << res.nAll << " (" << res.nSuccessPercents << "%) on iter: " << nSafeCicles << std::endl;
            m_vModificationModels[0]->update(rand() % 1000, rand() % 1000, rand() % 1000);
        }
        // if (nSafeCicles > 10000) {
        //     return 1;
        // }

        bna.randomModify(m_vModificationModels[nModModelNumber]);
        bna.removeAllDeadlockNodes();
        check_sum_result(bna, nextResult);
        if (res.nSuccess < nextResult.nSuccess) {
            res = nextResult;
            std::cout << " res: " << res.nSuccess << " / " << res.nAll << " (" << res.nSuccessPercents << "%) on iter: " << nSafeCicles << std::endl;
            bna.save("example_sum");
            bna.exportToDot("example_sum");
            bna.exportToCpp("example_sum");
            bna.exportToJavaScript("example_sum");
        } else {
            bna.load("example_sum");
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

