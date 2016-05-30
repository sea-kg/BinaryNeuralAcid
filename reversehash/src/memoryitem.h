#ifndef MEMORYITEM_H
#define MEMORYITEM_H

#include <QByteArray>

namespace crack_hash {
    class MemoryItem
    {
        public:
            MemoryItem();
            QByteArray input;
            QByteArray output;
    };
}

#endif // MEMORYITEM_H
