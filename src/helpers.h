#ifndef HELPERS_H
#define HELPERS_H
#include "memory.h"
#include <QString>

namespace reverse_hash {
    void convertArrayToVBool(QByteArray &in, QVector<bool> &vars, int size);
    void convertHEXStringToVBool(QString &in, QVector<bool> &vars, int size);
    QString convertVBoolHEXString(QVector<bool> &vars);
};

int loadPersent(QString filename);
void savePersent(QString filename, int nPersent);

#endif // HELPERS_H
