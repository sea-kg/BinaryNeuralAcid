#ifndef RHMEMORYITEM_H
#define RHMEMORYITEM_H

#include <QByteArray>
#include <QVector>


class RHMemoryItem {
    public:
        RHMemoryItem();
        QByteArray input;
        QByteArray output;
        const QVector<bool> &inputToVectorBool();
        const QVector<bool> &outputToVectorBool();

    private:
        void convertArrayToVBool(QByteArray &in, QVector<bool> &vars, int size);
        QVector<bool> m_vInput;
        QVector<bool> m_vOutput;
};


#endif // RHMEMORYITEM_H
