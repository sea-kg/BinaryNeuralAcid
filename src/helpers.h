#ifndef HELPERS_H
#define HELPERS_H
#include "memory.h"
#include <QString>

namespace reverse_hash {
    void convertArrayToVBool(QByteArray &in, QVector<bool> &vars, int size);
    void convertHEXStringToVBool(QString &in, QVector<bool> &vars, int size);
    QString convertVBoolHEXString(QVector<bool> &vars);
};

#endif // HELPERS_H
