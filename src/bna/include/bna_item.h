#ifndef BNA_ITEM_H
#define BNA_ITEM_H

#include <QString>
#include <QDataStream>

class BNAItem{
public:
    BNAItem(unsigned short x, unsigned short y, unsigned char t);
    BNAItem();
    unsigned short getX();
    unsigned short getY();
    unsigned char getT();

    void setX(unsigned short x);
    void setY(unsigned short y);
    void setT(unsigned char t);

    void readXYT(QDataStream &stream);
    void writeXYT(QDataStream &stream);

private:
    unsigned short m_nX;
    unsigned short m_nY;
    unsigned char m_cT;
};

#endif // BNA_ITEM_H
