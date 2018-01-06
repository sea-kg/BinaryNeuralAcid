#ifndef HELPERS_H
#define HELPERS_H
#include "memory.h"
#include <QString>

namespace reverse_hash {
    QString convertVBoolHEXString(QVector<bool> &vars);
};

int loadPersent(QString filename);
void savePersent(QString filename, int nPersent);

QString prepareName(int bitid);
QString prepareSubdir(int bitid);


#endif // HELPERS_H
