#include "and.h"

namespace reversehash {
	QString OperationAnd::type(){
		return QString("|");
	};
	bool OperationAnd::calc(bool b1, bool b2){
		return b1|b2;
	};
}
