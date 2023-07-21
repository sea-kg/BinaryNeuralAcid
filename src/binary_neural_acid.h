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

class BinaryNeuralAcidBit4 {
    public:
        BinaryNeuralAcidBit4(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2, BinaryNeuralAcidBit b3, BinaryNeuralAcidBit b4);
        void appendToVector(std::vector<BinaryNeuralAcidBit> &vars);
        BinaryNeuralAcidBit b1;
        BinaryNeuralAcidBit b2;
        BinaryNeuralAcidBit b3;
        BinaryNeuralAcidBit b4;
};

class BinaryNeuralAcidBitConvertor {
    public:
        static std::string toBinString(const BinaryNeuralAcidBit& bit);
        static std::string toBinString(const BinaryNeuralAcidBit4& bit4);
        static std::string toBinStringFromHexString(const std::string& sHex);
        static std::string toBinStringFromBits(const std::vector<BinaryNeuralAcidBit>& bits);
        static std::string toHexString(const BinaryNeuralAcidBit4& bit4);
        static std::string toHexStringFromBinString(const std::string& sBin);
        static std::string toHexStringFromBits(const std::vector<BinaryNeuralAcidBit>& bits);
        static std::vector<BinaryNeuralAcidBit> toBitsFromBinString(const std::string& sBin);
        static std::vector<BinaryNeuralAcidBit> toBitsFromHexString(const std::string& sHex);
};

template<class ValueType> class IBinaryNeuralAcidOperation {
    public:
        virtual std::string type() = 0;
        virtual ValueType calc(ValueType b1, ValueType b2) = 0;
};

class BinaryNeuralAcidOperationBitXor : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit>{
    public:
        virtual std::string type();
        virtual BinaryNeuralAcidBit calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2);
};

class BinaryNeuralAcidOperationBitNotXor : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit>{
    public:
        virtual std::string type();
        virtual BinaryNeuralAcidBit calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2);
};

class BinaryNeuralAcidOperationBitAnd : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> {
    public:
        virtual std::string type();
        virtual BinaryNeuralAcidBit calc(BinaryNeuralAcidBit b1, BinaryNeuralAcidBit b2);
};

class BinaryNeuralAcidOperationBitOr : public IBinaryNeuralAcidOperation<BinaryNeuralAcidBit> {
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

template<class ValueType> class BinaryNeuralAcidExpression {
    public:
        BinaryNeuralAcidExpression() {
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

class BinaryNeuralAcidGraphNodeInput {
    public:
        BinaryNeuralAcidGraphNodeInput(unsigned short nIndex);
        unsigned short getIndex();

    private:
        unsigned short m_nIndex;
};

class BinaryNeuralAcidGraphNodeOutput {
    public:
        BinaryNeuralAcidGraphNodeOutput(unsigned short nOutputIndex, unsigned short nInputNodeIndex);
        unsigned short getOutputIndex();
        unsigned short getInputNodeIndex();
        void setInputNodeIndex(unsigned short nInputNodeIndex);
        void writeToFile(std::ofstream &file);

    private:
        unsigned short m_nOutputIndex;
        unsigned short m_nInputNodeIndex;
};

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

class IBinaryNeuralAcidPseudoRandom {
    public:
        virtual unsigned int getNextRandom() = 0;
};

class BinaryNeuralAcidPseudoRandomStd : public IBinaryNeuralAcidPseudoRandom {
    public:
        BinaryNeuralAcidPseudoRandomStd();

        // IBinaryNeuralAcidPseudoRandom
        virtual unsigned int getNextRandom() override;
 };

class BinaryNeuralAcidPseudoRandomSin : public IBinaryNeuralAcidPseudoRandom {
    public:
        BinaryNeuralAcidPseudoRandomSin();
        void setInitSeed(unsigned int nSeed);
        unsigned int getInitSeed();
        unsigned int getSeed();

        // IBinaryNeuralAcidPseudoRandom
        virtual unsigned int getNextRandom() override;
    private:
        unsigned int m_nSeed;
        unsigned int m_nInitSeed;
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

class BinaryNeuralAcid {
	public:
		BinaryNeuralAcid();
        BinaryNeuralAcid(int nInput, int nOutput);
        ~BinaryNeuralAcid();
        unsigned int getInputSize();
        unsigned int getNodesSize();
        unsigned int getOutputSize();
        void setPseudoRandom(IBinaryNeuralAcidPseudoRandom *pRandom);
		bool load(const std::string &sFilename);
		bool save(const std::string &sFilename);
		void randomGenerate(const BinaryNeuralAcidConfig &config);
        int addNode(int nInX, int nInY, const std::string &sOperType);
        int addNodeOutput(int nIn);
        bool compile();
        void removeAllDeadlockNodes();

		bool exportToDot(const std::string &sFilename);
		bool exportToCpp(std::string filename, std::string funcname);

        const std::vector<BinaryNeuralAcidGraphNodeInput *> &getNodesInput();
        const std::vector<BinaryNeuralAcidGraphNode *> &getNodes();
        const std::vector<BinaryNeuralAcidGraphNodeOutput *> &getNodesOutput();
        int calculateDepth(int n);

        // QByteArray exportToByteArray();
        // void importFromByteArray(QByteArray data);
		nlohmann::json toJson();
        void randomModify(const BinaryNeuralAcidModificationModel *pModel);
        BinaryNeuralAcidBit calc(const std::vector<BinaryNeuralAcidBit> &vInputs, int nOutput);
        BinaryNeuralAcidBit compute(const std::vector<BinaryNeuralAcidBit> &vInputs, int nOutput);

        void compare(BinaryNeuralAcid &bna);

        static bool fileExists(const std::string &sFilename);
        static bool removeFile(const std::string &sFilename);
        static bool dirExists(const std::string &sDirname);

	private:
        std::string TAG;
        IBinaryNeuralAcidPseudoRandom *m_pRandom;
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
        std::vector<BinaryNeuralAcidGraphNodeInput *> m_vNodesInput;
        std::vector<BinaryNeuralAcidGraphNode *> m_vNodes;
        std::vector<BinaryNeuralAcidGraphNodeOutput *> m_vNodesOutput;

        void clearCalcExprsVars();
        void normalizeNodes();
        int removeDeadlockNodes();
        BinaryNeuralAcidVar<BinaryNeuralAcidBit> *getVarByIndex(int nIndex);
        std::vector<BinaryNeuralAcidExpression<BinaryNeuralAcidBit> *> m_vCalcExprs;
        std::vector<BinaryNeuralAcidVar<BinaryNeuralAcidBit> *> m_vCalcInputVars;
        std::vector<BinaryNeuralAcidVar<BinaryNeuralAcidBit> *> m_vCalcVars;
        std::vector<BinaryNeuralAcidVar<BinaryNeuralAcidBit> *> m_vCalcOutVars;
};

class BinaryNeuralAcidMemoryItem {
    public:
        BinaryNeuralAcidMemoryItem(int nInputBits, int nOutputBits);
        char* input;
        char* output;

    private:
        std::string TAG;
        int m_nInputBits;
        int m_nOutputBits;
        std::vector<BinaryNeuralAcidBit> m_vInput;
        std::vector<BinaryNeuralAcidBit> m_vOutput;
};

class BinaryNeuralAcidMemory {
    public:
        BinaryNeuralAcidMemory(int nInputBits, int nOutputBits);
        void load(std::string filename);
        void save(std::string filename);
        int size();
        BinaryNeuralAcidMemoryItem *at(int i);
        BinaryNeuralAcidMemoryItem *createItem();
        void append(BinaryNeuralAcidMemoryItem *pItem);
        void printData();
    private:
        std::string TAG;
        int m_nInputBits;
        int m_nOutputBits;
        std::vector<BinaryNeuralAcidMemoryItem *> m_vItems;
};

class BinaryNeuralAcidProject {

    public:
        BinaryNeuralAcidProject();
        bool open(std::string sDirPath);
        void setInputBits(int nInputBits);
        int getInputBits();
        void setOutputBits(int nOutputBits);
        int getOutputBits();
        void setDefaultCountNodes(int nDefaultCountNodes);
        bool create(std::string sDirPath);
        BinaryNeuralAcidMemory *getBNAMemory();
        void saveBNAMemory();
        int calculate(int bitid, bool bEnableSleep = false);
        void saveResult(int bitid, int nSuccess);
        int loadResult(int bitid);
        std::map<int,int> &getResults();
        BinaryNeuralAcid *getBNA(int bitid);
        void saveBNA(int bitid);

    private:
        std::string TAG;
        std::string prepareName(int bitid);
        std::string prepareSubdir(int bitid);
        void saveProjFile();
        void loadProjFile();

        std::string m_sDirPath;
        BinaryNeuralAcidMemory *m_pBNAMemory;
        int m_nInputBits;
        int m_nOutputBits;
        int m_nDefaultCountNodes;
        std::string m_sMemoryFileName;
        std::map<int,BinaryNeuralAcid *> m_mBNA;
        std::map<int,int> m_mapResults;
};


#endif // BINARY_NEURAL_ACID_H
