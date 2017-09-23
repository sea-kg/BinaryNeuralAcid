#ifndef MD5REVERT_HELPERS_H
#define MD5REVERT_HELPERS_H
#include <QString>
#include <QVector>


void convertHEXStringToVBool(QString &in, QVector<bool> &vars, int size);
QString convertVBoolHEXString(QVector<bool> &vars);

#endif // MD5REVERT_HELPERS_H
