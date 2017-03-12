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

	QString convertCharToHexCode(unsigned char c) {
		if(c == 0) return "0";
		if(c == 1) return "1";
		if(c == 2) return "2";
		if(c == 3) return "3";
		if(c == 4) return "4";
		if(c == 5) return "5";
		if(c == 6) return "6";
		if(c == 7) return "7";
		if(c == 8) return "8";
		if(c == 9) return "9";
		if(c == 10) return "a";
		if(c == 11) return "b";
		if(c == 12) return "c";
		if(c == 13) return "d";
		if(c == 14) return "e";
		if(c == 15) return "f";
		return "x";
	}

	// ----------------------------------------------------------------

    QString convertVBoolHEXString(QVector<bool> &vars) {
		QString result = "";
		unsigned char c = 0;
		for(int i = 0; i < vars.size(); i++){
			if(i % 4 == 0 && i != 0){
				result += convertCharToHexCode(c);
				c = 0x00;
			}
			c = (c << 1) | (vars[i] ? 0x01 : 0x00);
		}
		result += convertCharToHexCode(c);
        return result;
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
