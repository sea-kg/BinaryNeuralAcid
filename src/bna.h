#ifndef SEA5KG_BNA_H
#define SEA5KG_BNA_H

#include <map>
#include <json.hpp>
#include <iostream>
#include <fstream>
#include "bna_types.h"
#include "bna_operations.h"
#include "bna_expression.h"

void BNAConvertHEXStringToVBool(std::string &in, std::vector<BNABit> &vars, int size);
std::string BNAConvertVBoolHEXString(std::vector<BNABit> &vars);
std::string BNAConvertCharToHexCode(unsigned char c);
std::string BNAConvertHexToBin(std::string sHex);
std::string BNAConvertBinToHex(std::string sBin);

class BNANode {
    public:
        BNANode(unsigned short x, unsigned short y, const std::string &sOperationType);
        BNANode();
        unsigned short getX();
        unsigned short getY();
        std::string getOperationType();

        void setX(unsigned short x);
        void setY(unsigned short y);
        void setOperationType(const std::string &sOperationType);

        void readFromFile(std::ifstream &file);
        void writeToFile(std::ofstream &file);

    private:
        std::string TAG;
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

class BNA {
	public:
		BNA();
        BNA(int nInput, int nOutput);
        ~BNA();
		bool load(const std::string &sFilename);
		bool save(const std::string &sFilename);
		void randomGenerate(int nInput, int nOutput, int nSize);
        int addNode(int nInX, int nInY, const std::string &sOperType);
        bool compile();

		bool exportToDot(std::string filename, std::string graphname);
		bool exportToCpp(std::string filename, std::string funcname);
        
        const std::vector<BNANodeInput *> &getNodesInput();
        const std::vector<BNANode *> &getNodes();
        const std::vector<BNANodeOutput *> &getNodesOutput();

        // QByteArray exportToByteArray();
        // void importFromByteArray(QByteArray data);
		nlohmann::json toJson();
        void generateRandomMutations(int nRandomCicles);
        void appendRandomData(int nRandomCicles);
        BNABit calc(const std::vector<BNABit> &vInputs, int nOutput);

        unsigned int getInputSize();
        unsigned int getNodesSize();
        unsigned int getOutputSize();
        void compare(BNA &bna);

	private:
        std::string TAG;
        bool m_bCompiled;
        int m_nBnaVersion;
        bool readFromFileBna(std::ifstream &file);
        int readParam(std::ifstream &file, const std::string &sParamName);
        bool writeToFileBna(std::ofstream &file);

        bool registryOperationType(IBNAOper *pOper);
        std::map<std::string, IBNAOper *> m_vOperations;
        std::vector<IBNAOper *> m_vOperationList;
        int m_nOperSize;

        void clearResources();
        std::vector<BNANodeInput *> m_vNodesInput;
        std::vector<BNANode *> m_vNodes;
        std::vector<BNANodeOutput *> m_vNodesOutput;
        

        void clearCalcExprsVars();
        void normalizeInputNodes();
        BNAVar *getVarByIndex(int nIndex);
        std::vector<BNAExpression *> m_vCalcExprs;
        std::vector<BNAVar *> m_vCalcInputVars;
        std::vector<BNAVar *> m_vCalcVars;
        std::vector<BNAVar *> m_vCalcOutVars;
};

class BNAMemoryItem {
    public:
        BNAMemoryItem(int nInputBits, int nOutputBits);
        char* input;
        char* output;

    private:
        std::string TAG;
        int m_nInputBits;
        int m_nOutputBits;
        std::vector<BNABit> m_vInput;
        std::vector<BNABit> m_vOutput;
};

class BNAMemory {
    public:
        BNAMemory(int nInputBits, int nOutputBits);
        void load(std::string filename);
        void save(std::string filename);
        int size();
        BNAMemoryItem *at(int i);
        BNAMemoryItem *createItem();
        void append(BNAMemoryItem *pItem);
        void printData();
    private:
        std::string TAG;
        int m_nInputBits;
        int m_nOutputBits;
        std::vector<BNAMemoryItem *> m_vItems;
};

class BNAProject {

    public:
        BNAProject();
        bool open(std::string sDirPath);
        void setInputBits(int nInputBits);
        int getInputBits();
        void setOutputBits(int nOutputBits);
        int getOutputBits();
        void setDefaultCountNodes(int nDefaultCountNodes);
        bool create(std::string sDirPath);
        BNAMemory *getBNAMemory();
        void saveBNAMemory();
        int calculate(int bitid, bool bEnableSleep = false);
        void saveResult(int bitid, int nSuccess);
        int loadResult(int bitid);
        std::map<int,int> &getResults();
        BNA *getBNA(int bitid);
        void saveBNA(int bitid);

    private:
        std::string TAG;
        std::string prepareName(int bitid);
        std::string prepareSubdir(int bitid);
        void saveProjFile();
        void loadProjFile();

        std::string m_sDirPath;
        BNAMemory *m_pBNAMemory;
        int m_nInputBits;
        int m_nOutputBits;
        int m_nDefaultCountNodes;
        std::string m_sMemoryFileName;
        std::map<int,BNA *> m_mBNA;
        std::map<int,int> m_mapResults;
};


#endif // SEA5KG_BNA_H
