#ifndef SEA5KG_BNA_TEST_SIN1_H
#define SEA5KG_BNA_TEST_SIN1_H

#include "binary_neural_acid.h"
#include "render_window.h"

class BNATestSin1Item {
    public:
        BNATestSin1Item(float nIn, float nOut);
        float getIn();
        float getOut();

        const std::vector<BNABit> &getInOfBits();
        const std::vector<BNABit> &getOutOfBits();

    private:
        void floatToBNABits(const float &f, std::vector<BNABit> &vResult);

        float m_nIn;
        float m_nOut;

        std::vector<BNABit> m_vIn;
        std::vector<BNABit> m_vOut;
};

class BNATestSin1 : public ICallbacksRenderBNA<BNABit> {
    public:
        BNATestSin1();

        bool run();

        // ICallbacksRenderBNA
        virtual bool onStart() override;
        virtual void doMutation() override;
        virtual void doTestAndRevert() override;
        virtual BNAGroup<BNABit>* getBNA() override;
        virtual const BNAStatCalcResults *getResults() override;

    private:
        // void floatToByteArray(const float &f, unsigned char *pResult4);
        void byteArrayToFloat(const unsigned char *pBytes, float &nResult);
        void print(unsigned char *pResult4);
        void print(BNABit pResult[1]);
        void regenDataTests();
        bool loadDataTests();
        bool saveDataTests();
        void calculateCurrentCounters();
        
        std::string TAG;

        std::string m_sBNAFilename;
        BNAGroup<BNABit> *m_pBNA;

        std::string m_sDataTestsFilename;
        int m_nDataTestsSize;
        std::vector<BNATestSin1Item> m_vDataTests;
        
        BNAStatCalcResults *m_pResults;
        std::vector<BNAModificationModel *> m_vModificationModels;
        int m_nCurrentModificationModel;
        int m_nModificationModelCounter;
        int m_nSizeInput;
        int m_nSizeOutput;
};

#endif // SEA5KG_BNA_TEST_SIN1_H