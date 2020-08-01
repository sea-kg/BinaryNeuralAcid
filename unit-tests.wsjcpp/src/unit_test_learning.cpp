#include "unit_test_learning.h"
#include <vector>
#include <wsjcpp_core.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestLearning)

UnitTestLearning::UnitTestLearning()
    : WsjcppUnitTestBase("UnitTestLearning") {
}

// ---------------------------------------------------------------------

void UnitTestLearning::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestLearning::run() {
    bool bTestSuccess = true;
    compareB(bTestSuccess, "Not implemented", true, false);

    /*QString sFolder = "tmp_tests_bna_md5_2";
    BNAProject bnaProject;
    if(!bnaProject.open(sFolder)){
        std::cerr << "Could not open project, try create\n";
        bnaProject.setInputBits(128);
        bnaProject.setOutputBits(440);
        bnaProject.setDefaultCountNodes(256);
        bnaProject.create(sFolder);

        QVector<QString> tests;
        tests.push_back("123");
        tests.push_back("66^lvp/-!A");
        tests.push_back("YN!KAHfPjFU[\">IwHL");
        tests.push_back("admin");
        tests.push_back("adminadmin");
        tests.push_back("test");
        tests.push_back("test1234");
        tests.push_back("1234");
        tests.push_back("12345");
        tests.push_back("VeryHardPassword");

        // fill memory for learning
        BNAMemory *pBNAMemory = bnaProject.getBNAMemory();
        for(int i = 0; i < tests.size(); i++){
            BNAMemoryItem *pBNAMemoryItem = pBNAMemory->createItem();
            pBNAMemoryItem->output.append(tests[i]);
            pBNAMemoryItem->input = QCryptographicHash::hash(pBNAMemoryItem->output, QCryptographicHash::Md5);
            std::cout << pBNAMemoryItem->input.toHex().toStdString() << "\n";
            pBNAMemory->append(pBNAMemoryItem);
        }
        bnaProject.saveBNAMemory();
        return false;
    }

    int max_p = bnaProject.getBNAMemory()->size();

    for(int bitid = 0; bitid < bnaProject.getOutputBits(); bitid++) {
        int p = bnaProject.calculate(bitid);
        int prev_p = p;
        if(prev_p == max_p){
            continue;
        }
        std::cout << bitid << ": learning start... : " << p << "/" << max_p << "\n";
        BNA *pBNA = bnaProject.getBNA(bitid);
        int i = 0;
        QByteArray arrBNAKeep = pBNA->exportToByteArray();
        while(i < 1000){
            pBNA->generateRandomMutations(20);
            p = bnaProject.calculate(bitid);
            if(p > prev_p){
                arrBNAKeep = pBNA->exportToByteArray();
                bnaProject.saveBNA(bitid);
                prev_p = p;
                std::cout << bitid << " Updated to " << p << "/" << max_p << "\n";
            }else{
                pBNA->importFromByteArray(arrBNAKeep);
            }
            if(prev_p == max_p){
                break;
            }
            i++;
            if(i%100 == 0){
                QThread::msleep(200);
            }
        }
    }
    */

    return bTestSuccess;
}

