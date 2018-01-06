#ifndef RHMEMORY_H
#define RHMEMORY_H

#include<QString>
#include<QVector>
#include<bna.h>

class RHMemory {
    public:
        RHMemory();
        void load(QString filename);
        void save(QString filename);
        int size();
        BNAMemoryItem at(int i);
        void printData();
        void generateData(int nCount);
        void dataFrom(const QVector<QString> &vStrigns);
    private:
        QString alphabet();
        QString generateRandomString();
        int m_nInputSize;
        int m_nOutputSize;
        QVector<BNAMemoryItem> m_vItems;
};

#endif // RHMEMORY_H
