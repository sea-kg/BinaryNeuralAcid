#ifndef HELPERS_H
#define HELPERS_H
#include "graph.h"
#include "memory.h"
#include <QString>

namespace crack_hash {
    static int g_nCountInputMaxChars = 55;
    static int g_nCountInputBits = g_nCountInputMaxChars*8;
    QString alphabet();
    QString generateRandomString();
    void savePersent(QString filename, int);
    int loadPersent(QString filename);
    void convertArrayToVBool(QByteArray &in, QVector<bool> &vars, int size);
    void convertHEXStringToVBool(QString &in, QVector<bool> &vars, int size);
    QString convertVBoolHEXString(QVector<bool> &vars);
    int testGraph(crack_hash::Graph *pGraph, crack_hash::Memory *pMemory, int nBit);
}

#endif // HELPERS_H
