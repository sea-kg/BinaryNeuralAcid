#include "unit_test_random_mutation.h"
#include <vector>
#include <wsjcpp_core.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestRandomMutation)

UnitTestRandomMutation::UnitTestRandomMutation()
    : WsjcppUnitTestBase("UnitTestRandomMutation") {
}

// ---------------------------------------------------------------------

bool UnitTestRandomMutation::doBeforeTest() {
    return true;
}

// ---------------------------------------------------------------------


// void RandomMutations_Test::printStat(int i, int cicles, int max){
// 	std::cout << "\r\t Calculates " << i << " (cicles: " << cicles << " / " << max << ") ";
// }

// QVector<BNABit> createInputs(BNABit b1, BNABit b2, BNABit b3){
//     QVector<BNABit> vInputs;
//     vInputs.push_back(b1);
//     vInputs.push_back(b2);
//     vInputs.push_back(b3);
//     return vInputs;
// }

// struct DictonaryTest {
//     DictonaryTest(QVector<BNABit> vInputs, bool expectedOutput) :  vInputs(vInputs), expectedVal(expectedOutput) {}
//     QVector<BNABit> vInputs;
//     bool expectedVal;
// };

// int checkTest(BNA &bna, QVector< DictonaryTest *> &dict){
//     int nCounterSuccess = 0;
//     for(int i = 0; i < dict.size(); i++){
//         bool bResult = bna.calc(dict[i]->vInputs, 0);
//         bool bExpected = dict[i]->expectedVal;
//         // std::cout << "i:" << i << "    Result: " << (bResult ? "yes" : "no") << "; Expected: " << (bExpected ? "yes" : "no")  << "\n";
//         if((bool)bResult == (bool)bExpected){
//             nCounterSuccess++;
//         }
//     }
//     // std::cout << "nCounterSuccess: " << nCounterSuccess << "\n";
//     // for(int i = 0; i < dict.size(); i++){
//     //     unsigned char bResult = bna.calc(dict[i]->vInputs, 0);
//     //     bResult = bResult % 2;
//     //     unsigned char bExpected = dict[i]->expectedVal;
//     //     bExpected &= 0x01;
//     //     std::cout << "Result: " << (int)bResult << " " << (bResult ? "yes" : "no") << "; Expected: " << (int)bExpected << " " << (bExpected ? "yes" : "no")  << "\n";
//     //     std::cout << "Equals: " << (bResult == bExpected ? "yes" : "no")  << "\n";
//     // }
//     return nCounterSuccess;
// }

void UnitTestRandomMutation::executeTest() {
    compare("Not implemented", true, false);

    /* QVector< DictonaryTest *> dict;
    dict.push_back(new DictonaryTest(createInputs(B_1, B_1, B_1), true));
    dict.push_back(new DictonaryTest(createInputs(B_1, B_1, B_0), false));
    dict.push_back(new DictonaryTest(createInputs(B_1, B_0, B_1), true));
    dict.push_back(new DictonaryTest(createInputs(B_1, B_0, B_0), true));
    dict.push_back(new DictonaryTest(createInputs(B_0, B_1, B_1), false));
    dict.push_back(new DictonaryTest(createInputs(B_0, B_1, B_0), false));
    dict.push_back(new DictonaryTest(createInputs(B_0, B_0, B_1), false));
    dict.push_back(new DictonaryTest(createInputs(B_0, B_0, B_0), false));

    BNA bna;
    bna.randomGenerate(3,1,4);
    // bna.load("mutation_test.bna");
    // bna.appendRandomData(1);
    // bna.generateRandomMutations(5);

    //
    // bna.save("mutation_test.bna");
    int p = checkTest(bna, dict);
    // p = checkTest(bna, dict);
    bna.exportToDot("mutation_test.dot", "mutation_test");
    bna.generateRandomMutations(100);
    bna.exportToDot("mutation_test2.dot", "mutation_test2");

    // return true;
    int prev_p = p;
    std::cout << "Start... " << p << "\n";
    int nCicles = 0;
    QByteArray keep = bna.exportToByteArray();
    while(nCicles < 100000){
        bna.generateRandomMutations(100);
        bna.appendRandomData(1);
        p = checkTest(bna, dict);
        if(p > prev_p){
            std::cout << "Upgrade to " << p << " on step " << nCicles << "\n";
            prev_p = p;
            if(prev_p == 8){
                bna.save("mutation_test.bna");
                bna.exportToDot("mutation_test.dot", "mutation_test");
                break;
            }
        }else{
            // revert
            bna.importFromByteArray(keep);
        }
        nCicles++;
    }
    // std::cout << "New value: " << (bResult ? "yes" : "no" ) << "\n";
    return prev_p == 8;
    */
}

// ---------------------------------------------------------------------

bool UnitTestRandomMutation::doAfterTest() {
    return true;
}