#ifndef BNA_OPERS_H
#define BNA_OPERS_H

#include <QString>
#include <QMap>


class IBNAOper {
public:
    virtual QString type() = 0;
    virtual bool calc(bool b1, bool b2) = 0;
};

class BNAOperXor {
public:
    virtual QString type();
    virtual bool calc(bool b1, bool b2);
};

class BNAOperAnd {
public:
    virtual QString type();
    virtual bool calc(bool b1, bool b2);
};

class BNAOperOr {
public:
    virtual QString type();
    virtual bool calc(bool b1, bool b2);
};

QMap<unsigned char, IBNAOper *> g_mapOpers;

IBNAOper *getOper(unsigned char c);


#endif // BNA_OPERS_H
