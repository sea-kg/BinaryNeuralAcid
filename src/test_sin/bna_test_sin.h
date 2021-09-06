#ifndef SEA5KG_BNA_TEST_SIN_H
#define SEA5KG_BNA_TEST_SIN_H

#include "bna.h"
#include "render_window.h"

class BNATestSinItem {
    public:
        BNATestSinItem(float nIn, float nOut);
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


class BNATestSin : public ICallbacksRenderBNA {
    public:
        BNATestSin();

        bool run();

        // ICallbacksRenderBNA
        virtual bool onStart() override;
        virtual void doIterattion() override;
        virtual BNA* getBNA() override;

    private:
        // void floatToByteArray(const float &f, unsigned char *pResult4);
        void byteArrayToFloat(const unsigned char *pBytes, float &nResult);
        // void floatToBNABits(const float &f, BNABit pResult[32]);
        void BNABitsTofloat(const float &f, BNABit pResult[32]);
        void print(unsigned char *pResult4);
        void print(BNABit pResult[32]);
        void regenDataTests();
        bool loadDataTests();
        bool saveDataTests();
        int printCounters();
        void calculateCurrentCounters();
        
        std::string TAG;

        std::string m_sBNAFilename;
        BNA *m_pBNA;

        std::string m_sDataTestsFilename;
        int m_nDataTestsSize;
        std::vector<BNATestSinItem> m_vDataTests;

        std::vector<int> m_prevCounters;
        std::vector<int> m_currentCounters;
};

#endif // SEA5KG_BNA_TEST_SIN_H