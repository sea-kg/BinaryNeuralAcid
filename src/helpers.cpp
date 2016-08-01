#include "helpers.h"
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QCryptographicHash>

namespace reverse_hash {


    // ----------------------------------------------------------------

    void convertArrayToVBool(QByteArray &in, QVector<bool> &vars, int size) {
        vars.clear();
        for (int i = 0; i < in.size(); i++)
        {
            unsigned char ch = in.at(i);
            for (int r = 7; r >= 0; r--) {
                if (vars.size() > size)
                    return;
                unsigned char c = (int)ch >> r;
                c = c & 0x01;
                vars.push_back(c == 1 ? true : false);
            }
        }

        while (vars.size() < size) {
            vars.push_back(false);
        }
    }

    // ----------------------------------------------------------------

    QString convertVBoolHEXString(QVector<bool> &/*vars*/) {
        return "";
    }

    // ----------------------------------------------------------------

    void convertHEXStringToVBool(QString &in, QVector<bool> &vars, int size) {
        vars.clear();
        for (int i = 0; i < in.length(); i++)
        {
            QChar ch = in.at(i);
            if(ch == '0'){
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(false);
            }else if(ch == '1'){
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(true);
            }else if(ch == '2'){
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(false);
            }else if(ch == '3'){
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(true);
            }else if(ch == '4'){
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(false);
            }else if(ch == '5'){
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(true);
            }else if(ch == '6'){
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(false);
            }else if(ch == '7'){
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(true);
            }else if(ch == '8'){
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(false);
            }else if(ch == '9'){
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(false);
                vars.push_back(true);
            }else if(ch == 'a' || ch == 'A'){
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(false);
            }else if(ch == 'b' || ch == 'B'){
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(true);
                vars.push_back(true);
            }else if(ch == 'c' || ch == 'C'){
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(false);
            }else if(ch == 'd' || ch == 'D'){
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(false);
                vars.push_back(true);
            }else if(ch == 'e' || ch == 'E'){
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(false);
            }else if(ch == 'f' || ch == 'F'){
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(true);
                vars.push_back(true);
            }
        }

        while (vars.size() < size) {
            vars.push_back(false);
        }
    }
}
