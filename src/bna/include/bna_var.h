#ifndef BNA_VAR_H
#define BNA_VAR_H

#include <QString>


class BNAVar{
public:
    BNAVar();
    bool val();
    void val(bool bVal);
private:
    bool m_bVal;
};

#endif // BNA_VAR_H
