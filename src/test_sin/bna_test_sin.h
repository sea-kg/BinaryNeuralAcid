#ifndef SEA5KG_BNA_TEST_SIN_H
#define SEA5KG_BNA_TEST_SIN_H

#include "binary_neural_acid.h"
#include "render_window.h"

class BNATestSinItem {
    public:
        BNATestSinItem(float nIn, float nOut);
        float getIn();
        float getOut();

        const std::vector<BinaryNeuralAcidBit> &getInOfBits();
        const std::vector<BinaryNeuralAcidBit> &getOutOfBits();

    private:
        void floatToBNABits(const float &f, std::vector<BinaryNeuralAcidBit> &vResult);

        float m_nIn;
        float m_nOut;

        std::vector<BinaryNeuralAcidBit> m_vIn;
        std::vector<BinaryNeuralAcidBit> m_vOut;
};

class BNATestSin : public ICallbacksRenderBNA<BinaryNeuralAcidBit> {
    public:
        BNATestSin();

        bool run();

        // ICallbacksRenderBNA
        virtual bool onStart() override;
        virtual void doMutation() override;
        virtual void doTestAndRevert() override;
        virtual BNAGroup<BinaryNeuralAcidBit>* getBNA() override;
        virtual const BinaryNeuralAcidStatCalcResults *getResults() override;

    private:
        // void floatToByteArray(const float &f, unsigned char *pResult4);
        void byteArrayToFloat(const unsigned char *pBytes, float &nResult);
        // void floatToBNABits(const float &f, BinaryNeuralAcidBit pResult[32]);
        void BNABitsTofloat(const float &f, BinaryNeuralAcidBit pResult[32]);
        void print(unsigned char *pResult4);
        void print(BinaryNeuralAcidBit pResult[32]);
        void regenDataTests();
        bool loadDataTests();
        bool saveDataTests();
        void calculateCurrentCounters();
        
        std::string TAG;

        std::string m_sBNAFilename;
        BNAGroup<BinaryNeuralAcidBit> *m_pBNA;

        std::string m_sDataTestsFilename;
        int m_nDataTestsSize;
        std::vector<BNATestSinItem> m_vDataTests;
        
        BinaryNeuralAcidStatCalcResults *m_pResults;
        std::vector<BinaryNeuralAcidModificationModel *> m_vModificationModels;
        int m_nCurrentModificationModel;
        int m_nModificationModelCounter;
};

#endif // SEA5KG_BNA_TEST_SIN_H