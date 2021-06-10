#include "unit_test_reverse_test.h"
#include <vector>
#include <wsjcpp_core.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestReverseTest)

UnitTestReverseTest::UnitTestReverseTest()
    : WsjcppUnitTestBase("UnitTestReverseTest") {
}

// ---------------------------------------------------------------------

bool UnitTestReverseTest::doBeforeTest() {
    return false;
}

// ---------------------------------------------------------------------

// struct Reverse_Test_Struct{
//     Reverse_Test_Struct(QString md5, QString text) : md5(md5), text(text) {}
//     QString md5;
//     QString text;
// };

void UnitTestReverseTest::executeTest() {
    compare("Not implemented", true, false);

/*
    QString sFolder = "tmp_tests_bna_md5_2";

    std::vector<Reverse_Test_Struct> tests;
    tests.push_back(Reverse_Test_Struct("202CB962AC59075B964B07152D234B70", "123"));
    tests.push_back(Reverse_Test_Struct("92BA7B54A295FED5060A2BA44A72E595", "66^lvp/-!A"));
    tests.push_back(Reverse_Test_Struct("b885ca6d1c4e8231928ef2af5737426c", "YN!KAHfPjFU[\">IwHL"));
    tests.push_back(Reverse_Test_Struct("21232f297a57a5a743894a0e4a801fc3", "admin"));
    tests.push_back(Reverse_Test_Struct("f6fdffe48c908deb0f4c3bd36c032e72", "adminadmin"));
    tests.push_back(Reverse_Test_Struct("098f6bcd4621d373cade4e832627b4f6", "test"));
    tests.push_back(Reverse_Test_Struct("16d7a4fca7442dda3ad93c9a726597e4", "test1234"));
    tests.push_back(Reverse_Test_Struct("81dc9bdb52d04dc20036dbd8313ed055", "1234"));
    tests.push_back(Reverse_Test_Struct("b7608279dd8566a122364ea6eb44edc4", "VeryHardPassword"));
    tests.push_back(Reverse_Test_Struct("13334eaceaf87197c9cd2906b2f467fe", "jkdhsafkjdhf"));
    tests.push_back(Reverse_Test_Struct("09a6e6b94a8095ccaf18060fe8229f62", "VeryHard"));

    BNAProject bnaProject;
    if(!bnaProject.open(sFolder)){
        std::cerr << "Could not open project";
        return false;
    }

    for(int n = 0; n < tests.size(); n++){
        QString key = tests[n].md5;
        QString text = tests[n].text;
        QVector<BNABit> vInput;
        BNAConvertHEXStringToVBool(key, vInput, 128);
		
        QVector<BNABit> vOutputExpected;
        QByteArray originalString = text.toUtf8();
        BNAConvertArrayToVBool(originalString, vOutputExpected, 440);

        QVector<BNABit> vOutput;
        for (int bitid = 0; bitid < bnaProject.getOutputBits(); bitid++) {
            BNA *pBNA = bnaProject.getBNA(bitid);
            BNABit bResult = pBNA->calc(vInput, 0);
            vOutput.push_back(bResult);
		}

		int min = std::min(vOutputExpected.size(), vOutput.size());
		int nCheck = 0;
		for(int i = 0; i < min; i++){
			if(vOutput[i] == vOutputExpected[i])
				nCheck++;
		}
		for(int i = min; i < vOutput.size(); i++ ){
			if(vOutput[i] == false)
				nCheck++;
		}
        int  nPersent = (nCheck * 100) / bnaProject.getOutputBits();
        std::cout << "\t " << key.toStdString() << " => " << text.toStdString() << "; reverted " << nPersent << "% [" << nCheck << "/" << bnaProject.getOutputBits() << " bits]\n";
    }*/
}

// ---------------------------------------------------------------------

bool UnitTestReverseTest::doAfterTest() {
    return false;
}