#include "bna_opers.h"

QString BNAOperXor::type(){ return QString("^"); }
bool BNAOperXor::calc(bool b1, bool b2){ return b1^b2; }

QString BNAOperAnd::type(){ return QString("&"); }
bool BNAOperAnd::calc(bool b1, bool b2){ return b1&b2; }

QString BNAOperOr::type(){ return QString("|"); }
bool BNAOperOr::calc(bool b1, bool b2){ return b1|b2; }

