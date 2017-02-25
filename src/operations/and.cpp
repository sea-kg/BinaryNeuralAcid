#include "and.h"

namespace reversehash {
	
	OperationAnd::~OperationAnd(){
	}
	
	// -----------------------------------------------------------------
	
	QString OperationAnd::type(){
		return QString("|");
	};
	
	// -----------------------------------------------------------------
	
	bool OperationAnd::calc(bool b1, bool b2){
		return b1|b2;
	};
	
	// -----------------------------------------------------------------
}
