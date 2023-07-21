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

class BinaryNeuralAcidHelpers {
    public:
        static bool fileExists(const std::string &sFilename);
        static bool removeFile(const std::string &sFilename);
        static bool dirExists(const std::string &sDirname);
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

template<class ValueType> class BinaryNeuralAcid {
	public:
		BinaryNeuralAcid() {
            m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(0));
            m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(0, 0));
            registryOperationType(new BinaryNeuralAcidOperationBitXor());
            registryOperationType(new BinaryNeuralAcidOperationBitNotXor());
            registryOperationType(new BinaryNeuralAcidOperationBitAnd());
            registryOperationType(new BinaryNeuralAcidOperationBitOr());
            m_nOperSize = m_vOperationList.size();
            TAG = "BinaryNeuralAcid";
            m_nBnaVersion = 4;
            m_nBnaRevision = 0;
            m_pRandom = new BinaryNeuralAcidPseudoRandomStd();
        }

        BinaryNeuralAcid(int nInputSize, int nOutputSize) : BinaryNeuralAcid() {
            clearResources();
            for (int i = 0; i < nInputSize; i++) {
                m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(i));
            }
            // for (int i = 0; i < nOutputSize; i++) {
            //     m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(i, i));
            // }
            m_bCompiled = false;
        }

        ~BinaryNeuralAcid() {
            m_vOperations.clear();
            m_vOperationList.clear();
            m_vNodes.clear();
            delete m_pRandom;
            clearResources();
        }

        unsigned int getInputSize() {
            return m_vNodesInput.size();
        }

        unsigned int getNodesSize() {
            return m_vNodes.size();
        }

        unsigned int getOutputSize() {
            return m_vNodesOutput.size();
        }

        void setPseudoRandom(IBinaryNeuralAcidPseudoRandom *pRandom) {
            delete m_pRandom;
            m_pRandom = pRandom;
        }

		bool load(const std::string &sFilename) {
            clearResources();
            std::string sFilename0 = sFilename + ".bna";
            if (!BinaryNeuralAcidHelpers::fileExists(sFilename0)) {
                std::cerr << "load: file not exists '" << sFilename0 << "'" << std::endl;
                return false;
            }
            std::ifstream file;
            file.open(sFilename0, std::ios::in | std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "load: could not open file to read '" << sFilename0 << "'" << std::endl;
                return false;
            }
            bool bResult = readFromFileBna(file);
            file.close(); // TODO file will be automaticly closed on return of scope?

            m_bCompiled = false;
            compile();
            return bResult;
        }

		bool save(const std::string &sFilename) {
            std::string sFilename0 = sFilename + ".bna";
            if (BinaryNeuralAcidHelpers::fileExists(sFilename0)) {
                if (!BinaryNeuralAcidHelpers::removeFile(sFilename0)) {
                    std::cerr << "save: could not remove file: '" << sFilename0 << "'" << std::endl;
                    return false;
                }
            }
            std::ofstream file;
            file.open(sFilename0, std::ios::out | std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "save: could not open file to write: '" << sFilename0 << "'" << std::endl;
                return false;
            }

            m_nBnaRevision++;
            bool bResult = writeToFileBna(file);
            file.close(); // TODO file will be automaticly closed on return of scope?
            return bResult;
        }

		void randomGenerate(const BinaryNeuralAcidConfig &config) {
            clearResources();
            m_nBnaRevision = 0;
            for (int i = 0; i < config.getInputSize(); i++) {
                m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(i));
            }
            for (int i = 0; i < config.getNodesSize(); i++) {
                BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
                pItem->setX(m_pRandom->getNextRandom());
                pItem->setY(m_pRandom->getNextRandom());
                int nOper = m_pRandom->getNextRandom() % m_nOperSize;
                pItem->setOperationType(m_vOperationList[nOper]->type());
                m_vNodes.push_back(pItem);
            }
            for (int i = 0; i < config.getOutputSize(); i++) {
                m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(i, m_pRandom->getNextRandom()));
            }
            compile();
        }

        int addNode(int nInX, int nInY, const std::string &sOperType) {
            BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
            pItem->setX(nInX);
            pItem->setY(nInY);
            pItem->setOperationType(sOperType);
            m_vNodes.push_back(pItem);
            m_bCompiled = false;
            return m_vNodes.size() - 1 + m_vNodesInput.size();
        }

        int addNodeOutput(int nInputNodeId) {
            int nIndex = m_vNodesOutput.size();
            m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(nIndex, nInputNodeId));
            m_bCompiled = false;
            return nIndex;
        }

        bool compile() {
            if (m_bCompiled) {
                return true; // already compiled
            }

            // std::cout << "Compiling..." << std::endl;

            clearCalcExprsVars();

            normalizeNodes();

            // prepare input nodes
            for (unsigned int i  = 0; i < m_vNodesInput.size(); i++) {
                BinaryNeuralAcidVar<ValueType> *pVar = new BinaryNeuralAcidVar<ValueType>();
                pVar->setValue(B_0);
                pVar->setName("in" + std::to_string(i));
                m_vCalcInputVars.push_back(pVar);
            }

            int nItemsSize = m_vNodes.size();
            for (int i = 0; i < m_vNodes.size(); i++) {
                BinaryNeuralAcidExpression<ValueType> *pExpr = new BinaryNeuralAcidExpression<ValueType>();
                pExpr->setOperandLeft(getVarByIndex(m_vNodes[i]->getX()));
                pExpr->setOperandRight(getVarByIndex(m_vNodes[i]->getY()));
                if (m_vOperations.count(m_vNodes[i]->getOperationType()) == 0) {
                    throw std::runtime_error("Not found operation: " + m_vNodes[i]->getOperationType());
                }
                pExpr->oper(m_vOperations[m_vNodes[i]->getOperationType()]);
                BinaryNeuralAcidVar<ValueType> *pVar = new BinaryNeuralAcidVar<ValueType>();
                pVar->setName("node" + std::to_string(i));
                m_vCalcVars.push_back(pVar);
                pExpr->out(pVar);
                m_vCalcExpressions.push_back(pExpr);
            }

            for (int i = 0; i < m_vNodesOutput.size(); i++) {
                int nIndex = m_vNodesOutput[i]->getInputNodeIndex();
                m_vCalcOutVars.push_back(getVarByIndex(nIndex));
            }
            m_bCompiled = true;
            return true;
        }

        void removeAllDeadlockNodes() {
            clearCalcExprsVars();
            int nRemoved = removeDeadlockNodes();
            while (nRemoved > 0) {
                nRemoved = removeDeadlockNodes();
            }
            compile();
        }

		bool exportToDot(const std::string &sFilename) {
            std::string sFilename0 = sFilename + ".dot";
            if (BinaryNeuralAcidHelpers::fileExists(sFilename0)) {
                if (!BinaryNeuralAcidHelpers::removeFile(sFilename0)) {
                    std::cerr << "exportToDot: could not remove file: '" << sFilename0 << "'" << std::endl;
                    return false;
                }
            }
            std::ofstream file;
            file.open(sFilename0, std::ios::out | std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "exportToDot: could not open file to write: '" << sFilename0 << "'" << std::endl;
                return false;
            }

            file << "digraph BinaryNeuralAcid {" << "\n";
            for (int i = 0; i < m_vNodesInput.size(); i++) {
                file
                    << "    " << i << " [label=\"IN_" << i << "\"]"
                    << ";\n";
            }

            for (int i = 0; i < m_vNodes.size(); i++) {
                if (m_vNodes[i]->getOperationType() != "") {
                    file
                        << "    " << m_vNodes[i]->getId() << " [label=\"" << m_vNodes[i]->getId() << ":" << m_vNodes[i]->getOperationType() << "\"]"
                        << ";\n";
                }

                file
                    << "    " << m_vNodes[i]->getX()
                    << " -> " << m_vNodes[i]->getId()
                    << ";\n"
                ;
                file
                    << "    " << m_vNodes[i]->getY()
                    << " -> " << m_vNodes[i]->getId()
                    << ";\n"
                ;

            }
            for (int i = 0; i < m_vNodesOutput.size(); i++) {
                file
                    << "    " << "out" << m_vNodesOutput[i]->getOutputIndex() << " [label=\"OUT_" << m_vNodesOutput[i]->getOutputIndex() << "\"]"
                    << ";\n";
                file
                    << "    " << m_vNodesOutput[i]->getInputNodeIndex()
                    << " -> " << "out" << m_vNodesOutput[i]->getOutputIndex()
                    << ";\n"
                ;
            }
            file << "}" << "\n";
            return true;
        }

		bool exportToCpp(std::string filename, std::string funcname) {
            std::cerr << "TODO exportToCpp" << std::endl;
            /*QFile file(filename);
            QFileInfo fi(filename);
            if(fi.suffix() != "cpp"){
                std::cerr << "[ERROR]" << filename.toStdString() << " file must be have suffix 'cpp'\n";
                return false;
            }

            std::string filename_h = filename.left(filename.length() - 3);
            filename_h += "h";

            if (file.exists()) {
                file.remove();
            }

            QFile file_h(filename_h);
            if (file_h.exists()) {
                file_h.remove();
            }

            if ( !file.open(QIODevice::WriteOnly) ) {
                std::cerr << "Could not write file: " << filename.toStdString() << "\n";
                return false;
            }

            if ( !file_h.open(QIODevice::WriteOnly) ) {
                std::cerr << "Could not write file: " << filename_h.toStdString() << "\n";
                return false;
            }

            QTextStream stream_h( &file_h );
            stream_h << "#ifndef BNA_MD5_" << funcname.toUpper() << "_H\r\n";
            stream_h << "#define BNA_MD5_" << funcname.toUpper() << "_H\r\n\r\n";
            stream_h << "void " << funcname << "(";

            QTextStream stream( &file );
            stream << "#include \"" << fi.baseName() << ".h\"\r\n";
            stream << "void " << funcname << "(";
            for(unsigned int i = 0; i < m_vNodesInput.size(); i++){
                stream << "\r\n\tbool in" << i << ", ";
                stream_h << "\r\n\tbool in" << i << ", ";
            }

            for(unsigned  int i = 0; i < m_nOutput; i++){
                stream << "\r\n\tbool &out" << i;
                stream_h << "\r\n\tbool &out" << i;
                if(i < m_nOutput-1){
                    stream << ", ";
                    stream_h << ", ";
                }
            }
            stream << "\r\n) {\r\n";
            stream_h << "\r\n);\r\n\r\n";
            stream_h << "#endif //BNA_MD5_" << funcname.toUpper() << "_H\r\n";

            int nodes = m_vNodesInput.size();
            for(int i = 0; i < m_vNodes.size(); i++){
                std::string sX = (m_vNodes[i]->getX() < m_vNodesInput.size() ? "in" : "node") + std::to_string(m_vNodes[i]->getX());
                std::string sY = (m_vNodes[i]->getY() < m_vNodesInput.size() ? "in" : "node") + std::to_string(m_vNodes[i]->getY());
                std::string sNode = "node" + std::string::number(nodes); 
                stream << "\tbool " << sNode << " = " << sX << "|" << sY << ";\n";
                nodes++;
            }
            int out_nodes = nodes-m_nOutput;

            for(int i = out_nodes; i < nodes; i++){
                std::string sOut = "out" + std::to_string(i-out_nodes);
                std::string sNode = "node" + std::to_string(i);
                stream << "\t" << sOut << " = " << sNode << ";\n";
            }

            stream << "}\n";
            file.close();*/
            return true;
        }

        const std::vector<BinaryNeuralAcidGraphNodeInput *> &getNodesInput() {
            return m_vNodesInput;
        }

        const std::vector<BinaryNeuralAcidGraphNode *> &getNodes() {
            return m_vNodes;
        }

        const std::vector<BinaryNeuralAcidGraphNodeOutput *> &getNodesOutput() {
            return m_vNodesOutput;
        }

        int calculateDepth(int n) {
            int nNodesInputSize = m_vNodesInput.size();
            if (n < nNodesInputSize) {
                return 1;
            }
            n = n - nNodesInputSize;
            int nNodesSize = m_vNodes.size();
            if (n < nNodesSize) {
                BinaryNeuralAcidGraphNode *pNode = m_vNodes[n];
                int nLeft = this->calculateDepth(pNode->getX());
                int nRight = this->calculateDepth(pNode->getY());
                return std::max(nLeft, nRight) + 1;
            }
            n = n - nNodesSize;
            int nNodesOutputSize = m_vNodesOutput.size();
            if (n < nNodesOutputSize) {
                BinaryNeuralAcidGraphNodeOutput *pNode = m_vNodesOutput[n];
                return this->calculateDepth(pNode->getInputNodeIndex()) + 1;
            }
            return -1000;
        }

        // QByteArray exportToByteArray();
        // void importFromByteArray(QByteArray data);
		nlohmann::json toJson() {
            throw std::runtime_error("TODO toJson");
        }

        void randomModify(const BinaryNeuralAcidModificationModel *pModel) {
            for (int i = 0; i < pModel->getRemoveCicles(); i++) {
                if (m_vNodes.size() == 0) {
                    break;
                }
                int nIndex = m_pRandom->getNextRandom() % m_vNodes.size();
                BinaryNeuralAcidGraphNode *pItem = m_vNodes[nIndex];
                m_vNodes.erase(m_vNodes.begin() + nIndex);
                delete pItem;
            }
            for (int i = 0; i < pModel->getMutationCicles(); i++) {
                int nItemIndex = m_pRandom->getNextRandom() % (m_vNodes.size() + m_vNodesOutput.size());
                if (nItemIndex < m_vNodes.size()) {
                    m_vNodes[nItemIndex]->setX(m_pRandom->getNextRandom());
                    m_vNodes[nItemIndex]->setY(m_pRandom->getNextRandom());
                    int nOper = m_pRandom->getNextRandom() % m_nOperSize;
                    m_vNodes[nItemIndex]->setOperationType(m_vOperationList[nOper]->type());
                } else {
                    nItemIndex = nItemIndex - m_vNodes.size();
                    m_vNodesOutput[nItemIndex]->setInputNodeIndex(m_pRandom->getNextRandom());
                }
            }
            for (int i = 0; i < pModel->getAddCicles(); i++) {
                BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
                pItem->setX(m_pRandom->getNextRandom());
                pItem->setY(m_pRandom->getNextRandom());
                int nOper = m_pRandom->getNextRandom() % m_nOperSize;
                pItem->setOperationType(m_vOperationList[nOper]->type());
                m_vNodes.push_back(pItem);
            }
            if (pModel->getMutationCicles() > 0 || pModel->getAddCicles() > 0 || pModel->getRemoveCicles() > 0) {
                m_bCompiled = false;
                normalizeNodes();
            }
        }

        ValueType compute(const std::vector<ValueType> &vInputs, int nOutput) {
            if (!m_bCompiled) {
                compile();
            }
            if (vInputs.size() != m_vNodesInput.size()) {
                std::cerr << "[ERROR] invalid input count " << vInputs.size() << " (Expected: " << m_vNodesInput.size() << ") \n";
                return B_0;
            }
            for (unsigned int i  = 0; i < m_vNodesInput.size(); i++) {
                m_vCalcInputVars[i]->setValue(vInputs[i]);
            }
            for (unsigned int i = 0; i < m_vCalcExpressions.size(); i++) {
                m_vCalcExpressions[i]->exec();
            }
            return m_vCalcOutVars[nOutput]->getValue();
        }

        void compare(BinaryNeuralAcid<ValueType> &bna) {
            if(bna.getInputSize() != m_vNodesInput.size()){
                std::cout << "inputs not equals\n";
            }
            if(bna.getOutputSize() != m_vNodesOutput.size()){
                std::cout << "outputs not equals\n";
            }
            if(bna.m_vNodes.size() == m_vNodes.size()){
                for (int i = 0; i < m_vNodes.size(); i++) {
                    if (m_vNodes[i]->getOperationType() != bna.m_vNodes[i]->getOperationType()) {
                        std::cout << "\t T not equal in " << i << "\n";
                    }
                    if (m_vNodes[i]->getX() != bna.m_vNodes[i]->getX()) {
                        std::cout << "\t X not equal in " << i << "\n";
                    }
                    if (m_vNodes[i]->getY() != bna.m_vNodes[i]->getY()) {
                        std::cout << "\t Y not equal in " << i << "\n";
                    }
                }
            } else {
                std::cout << "Item size not equals " << bna.m_vNodes.size() << " != " << m_vNodes.size() << " \n";
            }
            if(bna.m_vCalcExpressions.size() == m_vCalcExpressions.size()){
                for(int i = 0; i < m_vCalcExpressions.size(); i++){
                    if(m_vCalcExpressions[i]->getOperandLeft()->getName() != bna.m_vCalcExpressions[i]->getOperandLeft()->getName()){
                        std::cout << "\t operand_left not equal in " << i << "\n";
                    }
                    if(m_vCalcExpressions[i]->op2()->getName() != bna.m_vCalcExpressions[i]->op2()->getName()){
                        std::cout << "\t operand_right not equal in " << i << "\n";
                    }
                    if(m_vCalcExpressions[i]->out()->getName() != bna.m_vCalcExpressions[i]->out()->getName()){
                        std::cout << "\t out not equal in " << i << "\n";
                    }
                    if(m_vCalcExpressions[i]->oper()->type() != bna.m_vCalcExpressions[i]->oper()->type()){
                        std::cout << "\t oper not equal in " << i << "\n";
                    }
                }
            } else {
                std::cout << "Exprs size not equals " << bna.m_vNodes.size() << " != " << m_vNodes.size() << " \n";
            }
        }

	private:
        std::string TAG;
        IBinaryNeuralAcidPseudoRandom *m_pRandom;
        bool m_bCompiled;
        int m_nBnaVersion;
        int m_nBnaRevision;

        bool readFromFileBna(std::ifstream &file) {
            clearResources();
            std::string sStr;
            file >> sStr;
            if (sStr != "BinaryNeuralAcid") {
                std::cerr << "readFromFileBna, is not a BinaryNeuralAcid file" << std::endl;
                return false;
            }
            int nBnaVersion = readParam(file, "version");
            if (nBnaVersion != m_nBnaVersion) {
                std::cerr << "readFromFileBna, Version expected '" << m_nBnaVersion << "', but got '" << nBnaVersion << "'" << std::endl;
                return false;
            }
            m_nBnaRevision = readParam(file, "revision");
            int nInputSize = readParam(file, "input");
            int nNodesSize = readParam(file, "nodes");
            int nOutputSize = readParam(file, "output");

            for (int i = 0; i < nInputSize; i++) {
                m_vNodesInput.push_back(new BinaryNeuralAcidGraphNodeInput(i));
            }

            // read nodes
            for (int i = 0; i < nNodesSize; i++) {
                BinaryNeuralAcidGraphNode *pItem = new BinaryNeuralAcidGraphNode();
                pItem->readFromFile(file);
                m_vNodes.push_back(pItem);
            }

            // read outputs
            for (int i = 0; i < nOutputSize; i++) {
                m_vNodesOutput.push_back(new BinaryNeuralAcidGraphNodeOutput(i, i));
                int nInputNodeIndex = readParam(file, "output");
                m_vNodesOutput[i]->setInputNodeIndex(nInputNodeIndex);
            }
            return compile(); // need for process expressions
        }

        int readParam(std::ifstream &file, const std::string &sParamName) {
            std::string sKeyword = "";
            file >> sKeyword;
            if (sKeyword != sParamName) {
                throw std::runtime_error("readParam, Expected keyword '" + sParamName + "'");
                return -1;
            }
            int nRet;
            file >> nRet;
            return nRet;
        }

        bool writeToFileBna(std::ofstream &file) {
            // basic information about file
            file << "BinaryNeuralAcid version " << m_nBnaVersion << "\n";
            file << "revision " << m_nBnaRevision << "\n";
            file << "input " << m_vNodesInput.size() << "\n";
            file << "nodes " << m_vNodes.size() << "\n";
            file << "output " << m_vNodesOutput.size() << "\n";
            for (int i = 0; i < m_vNodes.size(); i++) {
                m_vNodes[i]->writeToFile(file);
            }
            for (int i = 0; i < m_vNodesOutput.size(); i++) {
                m_vNodesOutput[i]->writeToFile(file);
            }
            return true;
        }

        bool registryOperationType(IBinaryNeuralAcidOperation<ValueType> *pOper) {
            // TODO check aready registered
            if (m_vOperations.count(pOper->type()) > 0) {
                throw std::runtime_error("Operation already registred");
            }
            m_vOperations[pOper->type()] = pOper;
            m_vOperationList.push_back(pOper);
            return true;
        }

        std::map<std::string, IBinaryNeuralAcidOperation<ValueType> *> m_vOperations;
        std::vector<IBinaryNeuralAcidOperation<ValueType> *> m_vOperationList;
        int m_nOperSize;

        void clearResources() {
            clearCalcExprsVars();
            for (int i = 0; i < m_vNodesInput.size(); i++) {
                delete m_vNodesInput[i];
            }
            m_vNodesInput.clear();
            for(int i = 0; i < m_vNodes.size(); i++){
                delete m_vNodes[i];
            }
            m_vNodes.clear();
            for(int i = 0; i < m_vNodesOutput.size(); i++){
                delete m_vNodesOutput[i];
            }
            m_vNodesOutput.clear();
        }
        std::vector<BinaryNeuralAcidGraphNodeInput *> m_vNodesInput;
        std::vector<BinaryNeuralAcidGraphNode *> m_vNodes;
        std::vector<BinaryNeuralAcidGraphNodeOutput *> m_vNodesOutput;

        void clearCalcExprsVars() {
            for (int i = 0; i < m_vCalcInputVars.size(); i++) {
                delete m_vCalcInputVars[i];
            }
            m_vCalcInputVars.clear();
            for (int i = 0; i < m_vCalcExpressions.size(); i++) {
                delete m_vCalcExpressions[i];
            }
            m_vCalcExpressions.clear();
            for (int i = 0; i < m_vCalcVars.size(); i++) {
                delete m_vCalcVars[i];
            }
            m_vCalcVars.clear();
            m_vCalcOutVars.clear();
            m_bCompiled = false;
        }

        void normalizeNodes() {
            int nNodesSize = m_vNodesInput.size();
            // normalize nodes
            for (int i = 0; i < m_vNodes.size(); i++) {
                m_vNodes[i]->setX(m_vNodes[i]->getX() % nNodesSize);
                m_vNodes[i]->setY(m_vNodes[i]->getY() % nNodesSize);
                m_vNodes[i]->setId(m_vNodesInput.size() + i);
                nNodesSize++;
            }
            int nNodesSize0 = m_vNodesInput.size() + m_vNodes.size();
            // normalize nodes output
            for (int i = 0; i < m_vNodesOutput.size(); i++) {
                m_vNodesOutput[i]->setInputNodeIndex(m_vNodesOutput[i]->getInputNodeIndex() % nNodesSize0);
            }
            removeDeadlockNodes();
        }

        int removeDeadlockNodes() {
            // clearCalcExprsVars();
            std::vector<int> vToRemoving;
            for (int i = 0; i < m_vNodes.size(); i++) {
                int nNodeIndex = i + m_vNodesInput.size();
                int nLinks = 0;
                for (int i0 = 0; i0 < m_vNodes.size(); i0++) {
                    if (m_vNodes[i0]->getX() == nNodeIndex || m_vNodes[i0]->getY() == nNodeIndex) {
                        nLinks++;
                    }
                }
                for (int i0 = 0; i0 < m_vNodesOutput.size(); i0++) {
                    if (m_vNodesOutput[i0]->getInputNodeIndex() == nNodeIndex) {
                        nLinks++;
                    }
                }
                if (nLinks == 0) {
                    // std::cout << "No links to node: nNodeIndex= " << nNodeIndex << std::endl;
                    vToRemoving.push_back(nNodeIndex);
                }
            }
            for (int i = vToRemoving.size() - 1; i >= 0; i--) {
                int nNodeIndex = vToRemoving[i];
                int nArrayIndex = vToRemoving[i] - m_vNodesInput.size();
                if (nNodeIndex > m_vNodes.size() + m_vNodesInput.size()) {
                    throw std::runtime_error("Node Index very big much");
                }
            }
            int nRemoved = 0;
            for (int i = vToRemoving.size() - 1; i >= 0; i--) {
                int nNodeIndex = vToRemoving[i];
                int nArrayIndex = vToRemoving[i] - m_vNodesInput.size();
                if (nArrayIndex < 0) {
                    continue;
                }
                BinaryNeuralAcidGraphNode *pNode = *(m_vNodes.begin() + nArrayIndex);
                m_vNodes.erase(m_vNodes.begin() + nArrayIndex);
                // delete pNode;
                for (int x = 0; x < m_vNodes.size(); x++) {
                    if (m_vNodes[x]->getX() >= nNodeIndex) {
                        m_vNodes[x]->setX(m_vNodes[x]->getX() - 1);
                    }
                    if (m_vNodes[x]->getY() >= nNodeIndex) {
                        m_vNodes[x]->setY(m_vNodes[x]->getY() - 1);
                    }
                }
                for (int x = 0; x < m_vNodesOutput.size(); x++) {
                    if (m_vNodesOutput[x]->getInputNodeIndex() >= nNodeIndex) {
                        m_vNodesOutput[x]->setInputNodeIndex(m_vNodesOutput[x]->getInputNodeIndex() - 1);
                    }
                }
                nRemoved++;
            }
            // fix ids
            for (int i = 0; i < m_vNodes.size(); i++) {
                m_vNodes[i]->setId(m_vNodesInput.size() + i);
            }
            return vToRemoving.size();
        }

        BinaryNeuralAcidVar<ValueType> *getVarByIndex(int nIndex) {
            if (nIndex < m_vCalcInputVars.size()) {
                return m_vCalcInputVars[nIndex];
            }
            nIndex = nIndex - m_vCalcInputVars.size();
            if (nIndex < m_vCalcVars.size()) {
                return m_vCalcVars[nIndex];
            }
            throw std::runtime_error("getVarByIndex: out of range index of var " + std::to_string(nIndex));
            nIndex = nIndex - m_vCalcVars.size();
            return m_vCalcOutVars[nIndex];
        }

        std::vector<BinaryNeuralAcidExpression<ValueType> *> m_vCalcExpressions;
        std::vector<BinaryNeuralAcidVar<ValueType> *> m_vCalcInputVars;
        std::vector<BinaryNeuralAcidVar<ValueType> *> m_vCalcVars;
        std::vector<BinaryNeuralAcidVar<ValueType> *> m_vCalcOutVars;
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
        BinaryNeuralAcid<BinaryNeuralAcidBit> *getBNA(int bitid);
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
        std::map<int,BinaryNeuralAcid<BinaryNeuralAcidBit> *> m_mBNA;
        std::map<int,int> m_mapResults;
};


#endif // BINARY_NEURAL_ACID_H
