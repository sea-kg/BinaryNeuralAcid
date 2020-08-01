#include "tests.h"
#include <QVector>
#include <iostream>
#include "bnaoper_test.h"
#include "callout_test.h"
#include "readwrite_test.h"
#include "reverse_test.h"
#include "learning_test.h"
#include "random_mutations_test.h"

bool runtests(){
	QVector<IReverseHashTest *> tests;
    tests.push_back(new BNAOper_Test());
    tests.push_back(new CallOut_Test());
	tests.push_back(new ReadWrite_Test());
    // tests.push_back(new Reverse_Test());
    // tests.push_back(new Learning_Test());
    tests.push_back(new RandomMutations_Test());

	bool bResult = true;
	for(int i = 0; i < tests.size(); i++){
		std::cout << " Run " << tests[i]->name().toStdString() << " ... \n";
		if(!tests[i]->run()){
			bResult	= false;
			std::cout << "  ->  [FAIL] \n";
		}else{
			std::cout << "  ->  [OK] \n";
		}
	}
	return bResult;
}
