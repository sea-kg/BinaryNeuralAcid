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
    // bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.addNode(0, 0, "XOR");
    bna.addNode(1, 1, "XOR");
    bna.addNode(2, 0, "AND");
    bna.addNode(2, 1, "OR");
    bna.addNode(4, 0, "XOR");
    bna.addNode(2, 4, "OR");
    bna.addNode(2, 4, "AND");
    bna.addNode(5, 7, "AND");
    bna.addNode(7, 6, "OR");
    bna.addNode(5, 8, "OR");
    bna.addNode(6, 4, "XOR");
    bna.addNode(6, 9, "AND");
    bna.addNode(3, 4, "AND");
    bna.addNode(10, 2, "AND");
    bna.addNode(9, 2, "AND");
    bna.addNode(11, 7, "OR");
    bna.addNode(1, 2, "AND");
    bna.addNode(3, 14, "AND");
    bna.addNode(11, 12, "AND");
    bna.addNode(11, 0, "AND");
    bna.addNode(16, 11, "AND");
    bna.addNode(5, 2, "OR");
    bna.addNode(8, 4, "AND");
    bna.addNode(15, 19, "AND");
    bna.addNode(3, 13, "XOR");
    bna.addNode(22, 25, "AND");
    bna.addNode(25, 9, "AND");
    bna.addNode(20, 6, "OR");
    bna.addNode(16, 24, "XOR");
    bna.addNode(15, 21, "AND");
    bna.addNode(29, 30, "OR");
    bna.addNode(31, 18, "AND");
    bna.addNode(28, 26, "AND");
    bna.addNode(27, 33, "AND");
    bna.addNode(9, 32, "AND");
    bna.addNode(23, 2, "XOR");
    bna.addNode(16, 35, "AND");
    bna.addNode(34, 17, "AND");
    bna.addNode(38, 37, "XOR");
    bna.addNode(39, 36, "XOR");
    bna.addNodeOutput(40);
    // bna.compile();

    bna.save(sPath0);
    bna.exportToDot(sPath0);

    // bna.randomGenerate(config);
    // bna.removeAllDeadlockNodes();


    // int nMutationCicles = 10;
    // int nAddCicles = 1000;
    // int nRemoveCicles = 20;
    // auto pModelModify = new BinaryNeuralAcidModificationModel(nMutationCicles, nAddCicles, nRemoveCicles);

    // bool bStop = false;
    // int nCicles = 0;
    // while (!bStop) {
    //     nCicles++;
    //     bStop = true;
    //     bna.randomModify(pModelModify);
    //     bna.removeAllDeadlockNodes();

    //     bna.save(sPath1);
    //     bna.exportToDot(sPath1);
    //     int sum = 0;
    //     for (int i = 0; i < 256; i++) {
    //         char c0 = i;
    //         unsigned char c = bna.compute({c0}, 0);
    //         sum += (int)c;
    //         std::cout << (unsigned int)(unsigned char)c0 << " => " << (int)c << std::endl;
    //     }
    //     if (sum == 0) {
    //         bStop = false;
    //     }
    // }
    // std::cout << "nCicles = " << nCicles << std::endl;

    for (int i = 0; i < 256; i++) {
        char c0 = i;
        unsigned char c = bna.compute({c0}, 0);
        unsigned char cExpected = c0;
        // std::cout << (unsigned int)(unsigned char)c0 << " => " << (int)c << std::endl;
        if (c != cExpected) {
            std::cerr << i << ", expected: " << (unsigned int)(unsigned char)cExpected << ", but got " << (unsigned int)(unsigned char)c << std::endl;
            return 1;
        }
    }

    return 0;
}