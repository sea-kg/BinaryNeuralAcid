#include "memory.h"
#include "helpers.h"
#include <iostream>
#include <cstring>
#include <QDataStream>
#include <QFile>
#include <QCryptographicHash>
#include <QDebug>

namespace reverse_hash {
    Memory::Memory(){
        m_nInputSize = 55; // 55 bytes
        m_nOutputSize = 16; // 16 bytes of md5 hash
    }

    // ----------------------------------------------------------------

    void Memory::load(QString filename){
        m_vItems.clear();
        QFile file(filename);
        if (!file.exists()) {
            qDebug().noquote().nospace() << "RHMEMORY:  File did not exists: " << filename;
            return;
        }
        if ( !file.open(QIODevice::ReadOnly) ) {
            qDebug().noquote().nospace() << "RHMEMORY: Could not open file " << filename;
            return;
        }

        QDataStream stream( &file );
        char *pFileType = new char[9];
        std::memset(pFileType, 0, 9);
        int nReaded = stream.readRawData(pFileType, 8);
        if(nReaded > 0){
            if(QString(pFileType) != "RHMEMORY"){
                qDebug().noquote().nospace() << "RHMEMORY: File type did not match with RHMEMORY. " << filename;
                return;
            }
        }else{
            qDebug().noquote().nospace() << "RHMEMORY: Could not read file (1) " << filename;
            return;
        }

        stream >> m_nInputSize;
        stream >> m_nOutputSize;
        int nCount;
        stream >> nCount;
        for(int i = 0; i < nCount; i++){
            MemoryItem memoryItem;
            stream >> memoryItem.input;
            stream >> memoryItem.output;
            m_vItems.push_back(memoryItem);
        }
    };

    // ----------------------------------------------------------------

    void Memory::save(QString filename){
        QFile file(filename);
        if (file.exists()) {
            file.remove();
        }
        if ( !file.open(QIODevice::WriteOnly) ) {
            qDebug().noquote().nospace() << "Could not write file: " << filename;
            return;
        }
        QDataStream stream( &file );
        stream.writeRawData("RHMEMORY", 8);
        stream << m_nInputSize << m_nOutputSize << m_vItems.size();
        for (int i = 0; i < m_vItems.size(); i++) {
            // QByteArray input(m_vItems[i].input);
            /*while(input.size() < m_nInputSize){
                input.append(char(0x00));
            }*/
            // stream.writeRawData(input.data(), input.size());
            // stream.writeRawData(m_vItems[i].output.data(), m_vItems[i].output.size());
            stream << m_vItems[i].input << m_vItems[i].output;
        }
        file.close();
    };

    // ----------------------------------------------------------------

    int Memory::size(){
        return m_vItems.size();
    }

    // ----------------------------------------------------------------

    MemoryItem Memory::at(int i){
        return m_vItems[i];
    };

    // ----------------------------------------------------------------

    void Memory::printData(){
        qDebug().noquote().nospace() <<  " --- Reverse Hash Memory --- ";
        for (int i = 0; i < m_vItems.size(); i++) {
            qDebug().noquote().nospace() << m_vItems[i].input.toHex() << " => " << m_vItems[i].output.toHex();
        }
    }

    // ----------------------------------------------------------------

    void Memory::generateData(int nCount){
        m_vItems.clear();
        for(int i = 0; i < nCount; i++){
            MemoryItem memoryItem;
            memoryItem.input.append(generateRandomString());
            memoryItem.output = QCryptographicHash::hash(memoryItem.input, QCryptographicHash::Md5);
            m_vItems.push_back(memoryItem);
        }
    };
    
    // ----------------------------------------------------------------
    
    QString Memory::alphabet() {
        return "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890[]{}:,.<>/?\"'\\*&^%$#!-+=";
    }
    
    // ----------------------------------------------------------------
    
    QString Memory::generateRandomString(){
        QString sAlphabet = alphabet();
        int len = qrand() % (m_nInputSize) + 2;
        QString str = "";
        for (int i = 0; i < len; i++) {
            str += sAlphabet[qrand() % sAlphabet.length()];
        }
        return str;
	}
}
