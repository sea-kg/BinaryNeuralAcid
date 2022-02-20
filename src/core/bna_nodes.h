#ifndef BNA_NODES_H
#define BNA_NODES_H

#include <map>
#include <json.hpp>
#include <iostream>
#include <fstream>
#include "bna_types.h"

class BNANode {
    public:
        BNANode(unsigned short x, unsigned short y, const std::string &sOperationType);
        BNANode();
        unsigned short getX();
        void setX(unsigned short nX);

        unsigned short getY();
        void setY(unsigned short nY);

        std::string getOperationType();
        void setOperationType(const std::string &sOperationType);

        unsigned short getId();
        void setId(unsigned short nId);

        void readFromFile(std::ifstream &file);
        void writeToFile(std::ofstream &file);

    private:
        std::string TAG;
        unsigned short m_nId;
        unsigned short m_nX;
        unsigned short m_nY;
        std::string m_sOperationType;
};

class BNANodeInput {
    public:
        BNANodeInput(unsigned short nIndex);
        unsigned short getIndex();

    private:
        unsigned short m_nIndex;
};

class BNANodeOutput {
    public:
        BNANodeOutput(unsigned short nOutputIndex, unsigned short nInputNodeIndex);
        unsigned short getOutputIndex();
        unsigned short getInputNodeIndex();
        void setInputNodeIndex(unsigned short nInputNodeIndex);
        void writeToFile(std::ofstream &file);

    private:
        unsigned short m_nOutputIndex;
        unsigned short m_nInputNodeIndex;
};

#endif // BNA_NODES_H
