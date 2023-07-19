#ifndef BINARY_NEURAL_ACID_H
#define BINARY_NEURAL_ACID_H

#include <map>
#include <json.hpp>
#include <iostream>
#include <fstream>

enum BinaryNeuralAcidBit : char {
    B_0 = 0x00,
    B_1 = 0x01
};

class BNABit4 {
    public:
        BNABit4(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2, BinaryNeuralAcidBit b3, BinaryNeuralAcidBit b4);
        void appendToVector(std::vector<BinaryNeuralAcidBit> &vars);
        BinaryNeuralAcidBit b1;
        BinaryNeuralAcidBit b2;
        BinaryNeuralAcidBit b3;
        BinaryNeuralAcidBit b4;
};

template<class ValueType> class IBinaryNeuralAcidOperation {
    public:
        virtual std::string type() = 0;
        virtual ValueType calc(ValueType b1, ValueType b2) = 0;
};

class BinaryNeuralAcidOperXor : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit>{
    public:
        virtual std::string type();
        virtual BinaryNeuralAcidBit calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2);
};

class BNAOperNotXor : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit>{
    public:
        virtual std::string type();
        virtual BinaryNeuralAcidBit calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2);
};

class BNAOperAnd : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> {
    public:
        virtual std::string type();
        virtual BinaryNeuralAcidBit calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2);
};

class BinaryNeuralAcidOperOr : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> {
    public:
        virtual std::string type();
        virtual BinaryNeuralAcidBit calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2);
};


template<class ValueType> class BinaryNeuralAcidVar {
    public:
        BinaryNeuralAcidVar() {
            // m_bVal = B_0;
            m_sName = "";
        }
        const std::string &getName() {
            return m_sName;
        }
        void setName(const std::string &sName) {
            m_sName = sName;
        }

        ValueType getValue() {
            return m_bVal;
        }
        void setValue(ValueType bVal) {
            m_bVal = bVal;
        }
    private:
        ValueType m_bVal;
        std::string m_sName;
};

template<class ValueType> class BNAExpression {
    public:
        BNAExpression() {
            m_pVar1 = nullptr;
            m_pVar2 = nullptr;
            m_pVarOut = nullptr;
            m_pOper = nullptr;
        }

        void setOperandLeft(BinaryNeuralAcidVar<ValueType> *pVar1) {
            m_pVar1 = pVar1;
        }

        BinaryNeuralAcidVar<ValueType> *getOperandLeft() {
            return m_pVar1;
        }

        void setOperandRight(BinaryNeuralAcidVar<ValueType> *pVar2) {
            m_pVar2 = pVar2;
        }

        BinaryNeuralAcidVar<ValueType> *op2() {
            return m_pVar2;
        }

        void oper(IBinaryNeuralAcidOperation<ValueType> *pOper) {
            m_pOper = pOper;
        }

        IBinaryNeuralAcidOperation<ValueType> *oper() {
            return m_pOper;
        }

        void out(BinaryNeuralAcidVar<ValueType> *pVarOut) {
            m_pVarOut = pVarOut;
        }

        BinaryNeuralAcidVar<ValueType> *out() {
            return  m_pVarOut;
        }

        void exec() {
            if (m_pVar1 == nullptr) {
                std::cerr << "[ERROR] m_pVar1 is NULL" << std::endl;
                return;
            }

            if (m_pVar2 == nullptr) {
                std::cerr << "[ERROR] m_pVar2 is NULL" << std::endl;
                return;
            }

            if (m_pVarOut == nullptr) {
                std::cerr << "[ERROR] m_pVarOut is NULL\n" << std::endl;
                return;
            }

            if (m_pOper == nullptr) {
                std::cerr << "[ERROR] m_pOper is NULL" << std::endl;
                return;
            }
            ValueType b1 = m_pVar1->getValue();
            ValueType b2 = m_pVar2->getValue();
            m_pVarOut->setValue(m_pOper->calc(b1, b2));
        }

    private:
        BinaryNeuralAcidVar<ValueType> *m_pVar1;
        BinaryNeuralAcidVar<ValueType> *m_pVar2;
        BinaryNeuralAcidVar<ValueType> *m_pVarOut;
        IBinaryNeuralAcidOperation<ValueType> *m_pOper;
};

// TODO class BinaryNeuralAcidGraph {

class BinaryNeuralAcidGraphNode {
    public:
        BinaryNeuralAcidGraphNode(unsigned short x, unsigned short y, const std::string &sOperationType);
        BinaryNeuralAcidGraphNode();
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


void BNAConvertHEXStringToVBool(std::string &in, std::vector<BinaryNeuralAcidBit> &vars, int size);
std::string BNAConvertVBoolHEXString(std::vector<BinaryNeuralAcidBit> &vars);
std::string BNAConvertCharToHexCode(unsigned char c);
std::string BNAConvertHexToBin(std::string sHex);
std::string BNAConvertBinToHex(std::string sBin);


class BinaryNeuralAcidStatCalcResults {
    public:
        BinaryNeuralAcidStatCalcResults(int nOutputSize);
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

class BinaryNeuralAcidModificationModel {
    public:
        BinaryNeuralAcidModificationModel();
        BinaryNeuralAcidModificationModel(int nMutationCicles, int nAddCicles, int nRemoveCicles);
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

class BinaryNeuralAcidConfig {
    public:
        BinaryNeuralAcidConfig();
        BinaryNeuralAcidConfig &setInputSize(int nInputSize);
        BinaryNeuralAcidConfig &setNodesSize(int nNodesSize);
        BinaryNeuralAcidConfig &setOutputSize(int nOutputSize);
        int getInputSize() const;
        int getNodesSize() const;
        int getOutputSize() const;
    private:
        int m_nInputSize;
        int m_nNodesSize;
        int m_nOutputSize;
};

class BNA {
	public:
		BNA();
        BNA(int nInput, int nOutput);
        ~BNA();
		bool load(const std::string &sFilename);
		bool save(const std::string &sFilename);
		void randomGenerate(const BinaryNeuralAcidConfig &config);
        int addNode(int nInX, int nInY, const std::string &sOperType);
        int addNodeOutput(int nIn);
        bool compile();

		bool exportToDot(std::string filename, std::string graphname);
		bool exportToCpp(std::string filename, std::string funcname);

        const std::vector<BNANodeInput *> &getNodesInput();
        const std::vector<BinaryNeuralAcidGraphNode *> &getNodes();
        const std::vector<BNANodeOutput *> &getNodesOutput();
        int calculateDepth(int n);

        // QByteArray exportToByteArray();
        // void importFromByteArray(QByteArray data);
		nlohmann::json toJson();
        void randomModify(const BinaryNeuralAcidModificationModel *pModel);
        BinaryNeuralAcidBit calc(const std::vector<BinaryNeuralAcidBit> &vInputs, int nOutput);
        BinaryNeuralAcidBit compute(const std::vector<BinaryNeuralAcidBit> &vInputs, int nOutput);

        unsigned int getInputSize();
        unsigned int getNodesSize();
        unsigned int getOutputSize();
        void compare(BNA &bna);

        static bool fileExists(const std::string &sFilename);
        static bool removeFile(const std::string &sFilename);
        static bool dirExists(const std::string &sDirname);

	private:
        std::string TAG;
        bool m_bCompiled;
        int m_nBnaVersion;
        int m_nBnaRevision;
        bool readFromFileBna(std::ifstream &file);
        int readParam(std::ifstream &file, const std::string &sParamName);
        bool writeToFileBna(std::ofstream &file);

        bool registryOperationType(IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *pOper);
        std::map<std::string, IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *> m_vOperations;
        std::vector<IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> *> m_vOperationList;
        int m_nOperSize;

        void clearResources();
        std::vector<BNANodeInput *> m_vNodesInput;
        std::vector<BinaryNeuralAcidGraphNode *> m_vNodes;
        std::vector<BNANodeOutput *> m_vNodesOutput;

        void clearCalcExprsVars();
        void normalizeNodes();
        BinaryNeuralAcidVar<BinaryNeuralAcidBit> *getVarByIndex(int nIndex);
        std::vector<BNAExpression<BinaryNeuralAcidBit> *> m_vCalcExprs;
        std::vector<BinaryNeuralAcidVar<BinaryNeuralAcidBit> *> m_vCalcInputVars;
        std::vector<BinaryNeuralAcidVar<BinaryNeuralAcidBit> *> m_vCalcVars;
        std::vector<BinaryNeuralAcidVar<BinaryNeuralAcidBit> *> m_vCalcOutVars;
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
        std::vector<BinaryNeuralAcidBit> m_vInput;
        std::vector<BinaryNeuralAcidBit> m_vOutput;
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


#endif // BINARY_NEURAL_ACID_H
