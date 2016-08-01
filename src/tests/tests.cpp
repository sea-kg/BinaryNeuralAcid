#include "tests.h"
#include <QVector>
#include <iostream>
#include "callout_test.h"
#include "readwrite_test.h"
#include "reverse_test.h"

bool runtests(){
	QVector<IReverseHashTest *> tests;
	tests.push_back(new CallOut_Test());
	tests.push_back(new ReadWrite_Test());
	tests.push_back(new Reverse_Test());
	
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
