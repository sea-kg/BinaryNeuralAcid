#ifndef HELPERS_H
#define HELPERS_H
#include <QString>

int loadPersent(QString filename);
void savePersent(QString filename, int nPersent);

QString prepareName(int bitid);
QString prepareSubdir(int bitid);


#endif // HELPERS_H
