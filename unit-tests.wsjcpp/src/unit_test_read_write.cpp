#include "unit_test_read_write.h"
#include <vector>
#include <wsjcpp_core.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestReadWrite)

UnitTestReadWrite::UnitTestReadWrite()
    : WsjcppUnitTestBase("UnitTestReadWrite") {
}

// ---------------------------------------------------------------------

void UnitTestReadWrite::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestReadWrite::run() {
    bool bTestSuccess = true;
    compareB(bTestSuccess, "Not implemented", true, false);
    /*
    BNA bna;
    bna.randomGenerate(128,1,256);

    qint64 size1 = 0, size2 = 0, size3 = 0, size4 = 0;

    QTemporaryFile file1;
    file1.setAutoRemove(true);
    if(file1.open()){
        file1.close();
        bna.save(file1.fileName());
        size1 = QFileInfo(file1.fileName()).size();
        // std::cout << "\t" << file1.fileName().toStdString() << " = " << int(size1) << " \n";
        bna.load(file1.fileName());
    }
    
    QTemporaryFile file2;
    file2.setAutoRemove(true);
    if(file2.open()){
        file2.close();
        bna.save(file2.fileName());
        size2 = QFileInfo(file2.fileName()).size();
        // std::cout << "\t" << file2.fileName().toStdString() << " = " << int(size2) << " \n";
        bna.load(file2.fileName());
    }
    
    QTemporaryFile file3;
    file3.setAutoRemove(true);
    if(file3.open()){
        file3.close();
        bna.save(file3.fileName());
        size3 = QFileInfo(file3.fileName()).size();
        // std::cout << "\t" << file3.fileName().toStdString() << " = " << int(size3) << " \n";
        bna.load(file3.fileName());
    }
    
    QTemporaryFile file4;
    file4.setAutoRemove(true);
    if(file4.open()){
        file4.close();
        bna.save(file4.fileName());
        size4 = QFileInfo(file4.fileName()).size();
        // std::cout << "\t" << file4.fileName().toStdString() << " = " << int(size4) << " \n";
        bna.load(file4.fileName());
    }    
	
	if(size1 == 0 || size2 == 0 || size3 == 0 || size4 == 0){
		return false;
	}
	
	if(size1 != size2 || size2 != size3 || size1 != size3 || size1 != size4 || size2 != size4 || size3 != size4){
		return false;
	}
    */
    return bTestSuccess;
}

