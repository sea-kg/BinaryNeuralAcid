#include "binary_neural_acid.h"

#include <vector>


int test_calculate_quallity(
    BinaryNeuralAcid<BinaryNeuralAcidBit> &bna,
    std::vector<BinaryNeuralAcidBit> &vInputs,
    std::vector<BinaryNeuralAcidBit> vOutputsExpected
) {
    std::vector<BinaryNeuralAcidBit> vOutputs;
    int nResult = 0;
    for (int i = 0; i < bna.getOutputSize(); i++) {
        BinaryNeuralAcidBit bit = bna.compute(vInputs, i);
        if (vOutputsExpected[i] == bit) {
            nResult++;
        }
    }
    return nResult;
}

float calculate_quallity(BinaryNeuralAcid<BinaryNeuralAcidBit> &bna, std::vector<std::pair<std::string, std::string>> &vTestData) {
    int nQuallity = 0;
    int nAllQuallity = 0;
    for (int t = 0; t < vTestData.size(); t++) {
        std::vector<BinaryNeuralAcidBit> vInputs = BinaryNeuralAcidBitConvertor::toBitsFromHexString(vTestData[t].first);
        std::vector<BinaryNeuralAcidBit> vOutputsExpected = BinaryNeuralAcidBitConvertor::toBitsFromHexString(vTestData[t].second);
        nQuallity += test_calculate_quallity(bna, vInputs, vOutputsExpected);
        nAllQuallity += vOutputsExpected.size();
    }
    float nQuallityPercent = (float(nQuallity) / float(nAllQuallity)) * 100;
    return nQuallityPercent;
}

int main() {

    std::vector<std::pair<std::string, std::string>> vTestData;

    vTestData.push_back(std::pair<std::string, std::string>("00000000000000000000000000000000", "cd9e459ea708a948d5c2f5a6ca8838cf"));
    vTestData.push_back(std::pair<std::string, std::string>("cd9e459ea708a948d5c2f5a6ca8838cf", "521214d453fe36d20ac094dec9916bbd"));
    vTestData.push_back(std::pair<std::string, std::string>("521214d453fe36d20ac094dec9916bbd", "5def46e8712a2b9eb859e52455d49799"));
    // vTestData.push_back(std::pair<std::string, std::string>("202CB962AC59075B964B07152D234B70", "123"));
    // vTestData.push_back(std::pair<std::string, std::string>("92BA7B54A295FED5060A2BA44A72E595", "66^lvp/-!A"));
    // vTestData.push_back(std::pair<std::string, std::string>("b885ca6d1c4e8231928ef2af5737426c", "YN!KAHfPjFU[\">IwHL"));
    // vTestData.push_back(std::pair<std::string, std::string>("21232f297a57a5a743894a0e4a801fc3", "admin"));
    // vTestData.push_back(std::pair<std::string, std::string>("f6fdffe48c908deb0f4c3bd36c032e72", "adminadmin"));
    // vTestData.push_back(std::pair<std::string, std::string>("098f6bcd4621d373cade4e832627b4f6", "test"));
    // vTestData.push_back(std::pair<std::string, std::string>("16d7a4fca7442dda3ad93c9a726597e4", "test1234"));
    // vTestData.push_back(std::pair<std::string, std::string>("81dc9bdb52d04dc20036dbd8313ed055", "1234"));
    // vTestData.push_back(std::pair<std::string, std::string>("b7608279dd8566a122364ea6eb44edc4", "VeryHardPassword"));
    // vTestData.push_back(std::pair<std::string, std::string>("13334eaceaf87197c9cd2906b2f467fe", "jkdhsafkjdhf"));
    // vTestData.push_back(std::pair<std::string, std::string>("09a6e6b94a8095ccaf18060fe8229f62", "VeryHard"));

    const std::string sPath0 = "./md5-test0";
    BinaryNeuralAcidConfig config;
    config
        .setInputSize(128)
        .setNodesSize(2000)
        .setOutputSize(128)
    ;

    BinaryNeuralAcid<BinaryNeuralAcidBit> bna;
    bna.setPseudoRandom(new BinaryNeuralAcidPseudoRandomSin());
    bna.randomGenerate(config);
    bna.removeAllDeadlockNodes();
    bna.save(sPath0);
    float nQuallityPercent = calculate_quallity(bna, vTestData);
    float nStartQuallityPercent = nQuallityPercent;
    std::cout << "Start quallity percent: " << nStartQuallityPercent << " (nodes: " << bna.getNodesSize() << ")" << std::endl;

    int nMutationCicles = 10;
    int nAddCicles = 1024;
    int nRemoveCicles = 20;
    auto pModelModify = new BinaryNeuralAcidModificationModel(nMutationCicles, nAddCicles, nRemoveCicles);

    int nCicle = 0;
    while (nCicle < 60) {
        nCicle++;
        bna.randomModify(pModelModify);
        bna.removeAllDeadlockNodes();
        float nQuallityPercentNew = calculate_quallity(bna, vTestData);
        // std::cout << nCicle << " quallity percent: " << nQuallityPercentNew << " (nodes: " << bna.getNodesSize() << ")" << std::endl;
        if (nQuallityPercentNew > nQuallityPercent) {
            // std::cout << nCicle << "    -> save " << std::endl;
            bna.save(sPath0);
            nQuallityPercent = nQuallityPercentNew;
        } else {
            // revert
            // std::cout << "    -> revert " << std::endl;
            bna.load(sPath0);
        }
    }

    std::cout << "Finish quallity percent: " << nQuallityPercent << " (nodes: " << bna.getNodesSize() << ")" << std::endl;
    bna.exportToDot(sPath0);
    if (nStartQuallityPercent >= nQuallityPercent) {
        std::cerr << ": No improvement found " << sPath0 << std::endl;
        return 1;
    }
    return 0;
}