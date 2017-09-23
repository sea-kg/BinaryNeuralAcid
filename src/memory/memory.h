#ifndef MEMORY_H
#define MEMORY_H

#include<QString>
#include<QVector>
#include"memoryitem.h"

namespace reverse_hash {

    class Memory
    {
        public:
            Memory();
            void load(QString filename);
            void save(QString filename);
            int size();
            MemoryItem at(int i);
            void printData();
            void generateData(int nCount);
        private:
			QString alphabet();
			QString generateRandomString();
            int m_nInputSize;
            int m_nOutputSize;
            QVector<MemoryItem> m_vItems;
    };
}
#endif // MEMORY_H
