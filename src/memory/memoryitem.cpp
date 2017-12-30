#include "memoryitem.h"

RHMemoryItem::RHMemoryItem() {
    // nothing
}

const QVector<bool> &RHMemoryItem::inputToVectorBool(){
    if(m_vInput.size() != input.size()*8){
        convertArrayToVBool(input, m_vInput, input.size()*8);
    }
    return m_vInput;
}

const QVector<bool> &RHMemoryItem::outputToVectorBool(){
    if(m_vOutput.size() != output.size()*8){
        convertArrayToVBool(output, m_vOutput, output.size()*8);
    }
    return m_vOutput;
}

void RHMemoryItem::convertArrayToVBool(QByteArray &in, QVector<bool> &vars, int size){
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
