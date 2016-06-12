#include "xor.h"

namespace reversehash {
	QString OperationXor::type(){
		return QString("^");
	};
	bool OperationXor::calc(bool b1, bool b2){
		return b1^b2;
	};
}
