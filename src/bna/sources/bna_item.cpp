#include <bna_item.h>

BNAItem::BNAItem(unsigned short x, unsigned short y, unsigned char t){
    m_nX = x;
    m_nY = y;
    m_cT = t;
}

// -----------------------------------------------------------------

BNAItem::BNAItem() {
    m_nX = 0;
    m_nY = 0;
    m_cT = 0;
}
		
// -----------------------------------------------------------------

unsigned short BNAItem::getX(){
    return m_nX;
}

// -----------------------------------------------------------------

unsigned short BNAItem::getY(){
    return m_nY;
}

// -----------------------------------------------------------------

unsigned char BNAItem::getT(){
    return m_cT;
}

// -----------------------------------------------------------------

void BNAItem::setX(unsigned short x){
    m_nX = x;
}

// -----------------------------------------------------------------

void BNAItem::setY(unsigned short y){
    m_nY = y;
}

// -----------------------------------------------------------------

void BNAItem::setT(unsigned char t){
    m_cT = t;
}

// -----------------------------------------------------------------

void BNAItem::readXYT(QDataStream &stream){
    stream >> m_nX;
    stream >> m_nY;
    stream >> m_cT;
}

// -----------------------------------------------------------------

void BNAItem::writeXYT(QDataStream &stream){
    stream << m_nX << m_nY << m_cT;
}

// -----------------------------------------------------------------

