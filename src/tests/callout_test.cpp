#include "callout_test.h"
#include <bna.h>
#include <iostream>

QString CallOut_Test::name(){
	return "CallOut_Test";
};

bool CallOut_Test::run(){
    BNA bna;
    bna.randomGenerate(15, 1, 100);
    QVector<bool> vInputs;
    for(int i = 0; i < 15; i++){
        vInputs.push_back(true);
    }

    bool bResult = bna.calc(vInputs, 1);
    return bResult;
}

