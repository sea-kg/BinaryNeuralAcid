#ifndef BNA_H
#define BNA_H

#include <map>
#include <json.hpp>
#include <iostream>
#include <fstream>
#include "bna_types.h"
#include "bna_operations.h"
#include "bna_expression.h"
#include "bna_nodes.h"

void BNAConvertHEXStringToVBool(std::string &in, std::vector<BNABit> &vars, int size);
std::string BNAConvertVBoolHEXString(std::vector<BNABit> &vars);
std::string BNAConvertCharToHexCode(unsigned char c);
std::string BNAConvertHexToBin(std::string sHex);
std::string BNAConvertBinToHex(std::string sBin);


class BNAStatCalcResults {
    public:
        BNAStatCalcResults(int nOutputSize);
        const std::vector<int> &getPrevCounters() const;
        const std::vector<int> &getPrevCountersPercents() const;
        int getAllPrevCountersPercents() const;
        void setPrevCounters(const std::vector<int> &vValues);
        const std::vector<int> &getCurrentCounters() const;
        const std::vector<int> &getCurrentCountersPercents() const;
        int getAllCurrentCountersPercents() const;
        void setCurrentCounters(const std::vector<int> &vValues);
        void resetCurrentCounters();
        void incrementCurrentCounter(int nIndex);
        void calcPercents(int nDataTestsSize);
        int getSummaryDiff();

    private:
        std::string TAG;
        int m_nOutputSize;
        int m_nDataTestsSize;
        std::vector<int> m_vPrevCounters;
        std::vector<int> m_vPrevCountersPercents;
        int m_nAllPrevCountersPercents;
        std::vector<int> m_vCurrentCounters;
        std::vector<int> m_vCurrentCountersPercents;
        int m_nAllCurrentCountersPercents;
        int m_nSummaryDiff;
};

class BNAModificationModel {
    public:
        BNAModificationModel();
        BNAModificationModel(int nMutationCicles, int nAddCicles, int nRemoveCicles);
        void update(int nMutationCicles, int nAddCicles, int nRemoveCicles);
        void reset();
        int getMutationCicles() const;
        int getAddCicles() const;
        int getRemoveCicles() const;
        void print() const;

    private:
        int m_nMutationCicles;
        int m_nAddCicles;
        int m_nRemoveCicles;
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
        int calculateDepth(int n);

        // QByteArray exportToByteArray();
        // void importFromByteArray(QByteArray data);
		nlohmann::json toJson();
        void randomModify(const BNAModificationModel *pModel);
        BNABit calc(const std::vector<BNABit> &vInputs, int nOutput);

        unsigned int getInputSize();
        unsigned int getNodesSize();
        unsigned int getOutputSize();
        void compare(BNA &bna);

	private:
        std::string TAG;
        bool m_bCompiled;
        int m_nBnaVersion;
        int m_nBnaRevision;
        bool readFromFileBna(std::ifstream &file);
        int readParam(std::ifstream &file, const std::string &sParamName);
        bool writeToFileBna(std::ofstream &file);

        bool registryOperationType(IBNAOper<BNABit> *pOper);
        std::map<std::string, IBNAOper<BNABit> *> m_vOperations;
        std::vector<IBNAOper<BNABit> *> m_vOperationList;
        int m_nOperSize;

        void clearResources();
        std::vector<BNANodeInput *> m_vNodesInput;
        std::vector<BNANode *> m_vNodes;
        std::vector<BNANodeOutput *> m_vNodesOutput;

        void clearCalcExprsVars();
        void normalizeNodes();
        BNAVar<BNABit> *getVarByIndex(int nIndex);
        std::vector<BNAExpression<BNABit> *> m_vCalcExprs;
        std::vector<BNAVar<BNABit> *> m_vCalcInputVars;
        std::vector<BNAVar<BNABit> *> m_vCalcVars;
        std::vector<BNAVar<BNABit> *> m_vCalcOutVars;
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


#endif // BNA_H