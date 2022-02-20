#ifndef BNA_GROUP_H
#define BNA_GROUP_H

#include <map>
#include <json.hpp>
#include <iostream>
#include <fstream>
#include "bna_types.h"
#include "bna_operations.h"
#include "bna_expression.h"
#include "bna.h"

class BNAGroup {
    public:
        BNAGroup();
        BNAGroup(int nInput, int nOutput);
        ~BNAGroup();
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
        void addRandomNodes(int nRandomCicles);
        void removeRandomNodes(int nRandomCicles);
        BNABit calc(const std::vector<BNABit> &vInputs, int nOutput);

        unsigned int getInputSize();
        unsigned int getNodesSize();
        unsigned int getOutputSize();
        void compare(BNAGroup &bna);

	private:
        std::string TAG;
        bool m_bCompiled;
        int m_nBnaVersion;
        int m_nBnaRevision;
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
        void normalizeNodes();
        BNAVar *getVarByIndex(int nIndex);
        std::vector<BNAExpression *> m_vCalcExprs;
        std::vector<BNAVar *> m_vCalcInputVars;
        std::vector<BNAVar *> m_vCalcVars;
        std::vector<BNAVar *> m_vCalcOutVars;
};

#endif // BNA_GROUP_H
