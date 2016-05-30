#include "helpers.h"
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QCryptographicHash>

namespace crack_hash {

    QString alphabet() {
        return "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890[]{}:,.<>/?\"'\\*&^%$#!-+=";
    }

    QString generateRandomString() {
        QString sAlphabet = alphabet();
        int len = qrand() % (crack_hash::g_nCountInputBits/8) + 2;
        QString str = "";
        for (int i = 0; i < len; i++) {
            str += sAlphabet[qrand() % sAlphabet.length()];
        }
        return str;
    };

    // ----------------------------------------------------------------

    void savePersent(QString filename, int nPersent) {
        QFile file(filename);
        if (file.exists()) {
            file.remove();
        }
        if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
            std::cout << "error save " << filename.toStdString() << "\n";
            return;
        }
        QTextStream stream( &file );
        stream << QString::number(nPersent);
        file.close();
    }

    // ----------------------------------------------------------------

    int loadPersent(QString filename) {
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly)) {
            std::cout << "error load " << filename.toStdString() << "\n";
            return 0;
        }

        int nPersent = 0;
        QTextStream in(&file);
        if(!in.atEnd()) {
            QString line = in.readLine();
            nPersent = line.toInt();
        }
        file.close();
        return nPersent;
    }

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

    // ----------------------------------------------------------------

    int testGraph(crack_hash::Graph *pGraph, crack_hash::Memory *pMemory, int nBit) {
        crack_hash::Calculate *pCalc = new crack_hash::Calculate(pGraph);
        int nCountExperiments = pMemory->size();
        int nGoodResults = 0;

        std::cout << "\rSuccessful restorations ... 0% (from 0)";

        for (int t = 0; t < nCountExperiments; t++) {

            MemoryItem memoryItem = pMemory->at(t);

            QVector<bool> str;
            crack_hash::convertArrayToVBool(memoryItem.input, str, memoryItem.input.size()*8);
            while (str.size() < nBit) {
                str.push_back(false);
            };

            QVector<bool> hash;
            crack_hash::convertArrayToVBool(memoryItem.output, hash, 128);

            QMap<QString, bool> input;
            for (int i = 0; i < 128; i++) {
                QString in = "i" + QString::number(i, 16).rightJustified(2, '0');
                input[in] = hash[i];
            }
            bool bResult = pCalc->calc("o00", input);

            if (t > 0) {
                int nPersent = (nGoodResults * 100) / (t);
                std::cout << "\rSuccessful restorations ... " << nPersent << "% (from " << t << ")             ";
            }

            if (str[nBit] == bResult) {
                nGoodResults++;
            }
        }

        int nPersent = (nGoodResults * 100) / nCountExperiments;
        std::cout << "\rSuccessful restorations ... " << nPersent << "%                              \n";
        return nPersent;
    }
}
