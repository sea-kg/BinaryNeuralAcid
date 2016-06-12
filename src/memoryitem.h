#ifndef MEMORYITEM_H
#define MEMORYITEM_H

#include <QByteArray>
#include <QVector>

namespace crack_hash {
    class MemoryItem
    {
        public:
            MemoryItem();
            QByteArray input;
            QByteArray output;
            const QVector<bool> &inputToVectorBool();
			const QVector<bool> &outputToVectorBool();

		private:
			void convertArrayToVBool(QByteArray &in, QVector<bool> &vars, int size);
			QVector<bool> m_vInput;
			QVector<bool> m_vOutput;
    };
}

#endif // MEMORYITEM_H
