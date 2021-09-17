
#include "bna_test_sin.h"
#include <wsjcpp_core.h>

// -----------------------------------------------------------------
// BNATestSinItem

BNATestSinItem::BNATestSinItem(float nIn, float nOut) {
    m_nIn = nIn;
    m_nOut = nOut;
    floatToBNABits(m_nIn, m_vIn);
    floatToBNABits(m_nOut, m_vOut);
}

float BNATestSinItem::getIn() {
    return m_nIn;
}

float BNATestSinItem::getOut() {
    return m_nOut;
}

const std::vector<BNABit> &BNATestSinItem::getInOfBits() {
    return m_vIn;
}

const std::vector<BNABit> &BNATestSinItem::getOutOfBits() {
    return m_vOut;
}

void BNATestSinItem::floatToBNABits(const float &f, std::vector<BNABit> &vResult) {
    vResult.resize(32);

    unsigned char const * b = reinterpret_cast<unsigned char const *>(&f);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        unsigned char c = b[i];
        for (int x = 0; x < 8; x++) {
            vResult[(7 - x) + i*8] = (BNABit)(c >> x & 0x01);
        }
    }
}

// -----------------------------------------------------------------
// BNATestSin

BNATestSin::BNATestSin() {
    TAG = "BNATestSin";
    // in 32 bits
    // out 32 bits
    m_sBNAFilename = "testsin";
    m_pBNA = new BNA(32,32);
    m_sDataTestsFilename = "testsin.bnadatatest";
    m_nDataTestsSize = 2000;
    m_pResults = new BNAStatCalcResults(32);

    m_vModificationModels.push_back(new BNAModificationModel(rand() % 15, rand() % 15, rand() % 15));
    m_vModificationModels.push_back(new BNAModificationModel(1,0,0));
    m_vModificationModels.push_back(new BNAModificationModel(0,10,0));
    m_vModificationModels.push_back(new BNAModificationModel(0,0,1));
    m_nCurrentModificationModel = 0;
    m_nModificationModelCounter = 0;

}

bool BNATestSin::run() {
    RenderBNA render(this);
    return render.run();
}

bool BNATestSin::onStart() {
    // 1. load last bna (or generate randomly)
    if (WsjcppCore::fileExists(m_sBNAFilename + ".bna")) {
        if (!m_pBNA->load(m_sBNAFilename)) {
            std::cout << "ERROR Could not load file " << m_sBNAFilename << ".bna" << std::endl;
            return false;
        }
    } else {
        m_pBNA->randomGenerate(32,32, 128);
        m_pBNA->save(m_sBNAFilename);
    }

    // 2. load data tests (or generate new)
    if (!loadDataTests()) {
        regenDataTests();
        saveDataTests();
    }

    // init prev counters
    calculateCurrentCounters();
    m_pResults->setPrevCounters(m_pResults->getCurrentCounters());
    return true;
}

void BNATestSin::doMutation() {
    
    m_nModificationModelCounter++;
    if (m_nModificationModelCounter > 100) {
        m_nModificationModelCounter = 0;
        m_nCurrentModificationModel += 1;
        m_nCurrentModificationModel = m_nCurrentModificationModel % m_vModificationModels.size();
        if (m_nCurrentModificationModel == 0) {
            m_vModificationModels[0]->update(rand() % 15, rand() % 15, rand() % 15);
        }
    }
    m_pBNA->randomModify(m_vModificationModels[m_nCurrentModificationModel]);
}

void BNATestSin::doTestAndRevert() {
    calculateCurrentCounters();

    // m_pModificationModel

    if (m_pResults->getSummaryDiff() > 0) {
        // oo! nice current calculation better
        m_pBNA->save(m_sBNAFilename);
        m_pResults->setPrevCounters(m_pResults->getCurrentCounters());
    } else {
        // std::cout << "Reset bna " << std::endl;
        m_pBNA->load(m_sBNAFilename);
    }
}

BNA* BNATestSin::getBNA() {
    return m_pBNA;
}

const BNAStatCalcResults *BNATestSin::getResults() {
    return m_pResults;
}

void BNATestSin::byteArrayToFloat(const unsigned char *pBytes, float &nResult) {
    std::memcpy(&nResult, pBytes, sizeof(nResult));
}

void BNATestSin::print(unsigned char *pResult4) {
    for (std::size_t i = 0; i != 4; ++i) {
        std::printf("The byte #%zu is 0x%02X\n", i, pResult4[i]);
    }
}

void BNATestSin::print(BNABit pResult[32]) {
    for (int i = 0; i < 32; i++) {
        std::cout << (pResult[i] == BNABit::B_0 ? "0" : "1");
        if ( (i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void BNATestSin::regenDataTests() {
    m_vDataTests.clear();
    
    float HI = 1.0f;
    float LO = -1.0f;
    for (int i = 0; i < m_nDataTestsSize; i++) {
        float r3 = LO + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(HI - LO)));
        float sin_r3 = sin(r3);
        m_vDataTests.push_back(BNATestSinItem(r3, sin_r3));
    }
}

bool BNATestSin::loadDataTests() {
    std::string sFilename = "testsin.bnadatatest";
    if (!WsjcppCore::fileExists(sFilename)) {
        return false;
    }

    std::ifstream file;
    file.open(sFilename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        WsjcppLog::err(TAG, "load: could not open file to read '" + sFilename + "'");
        return false;
    }
    
    float nIn;
    float nOut;
    for (int i = 0; i < m_nDataTestsSize; i++) {
        file >> nIn;
        file >> nOut;
        m_vDataTests.push_back(BNATestSinItem(nIn, nOut));
    }
    return true;
}

bool BNATestSin::saveDataTests() {
    if (WsjcppCore::fileExists(m_sDataTestsFilename)) {
        if (!WsjcppCore::removeFile(m_sDataTestsFilename)) {
            WsjcppLog::err(TAG, "save: could not remove file '" + m_sDataTestsFilename + "'");
            return false;
        }
    }
    std::ofstream file;
    file.open(m_sDataTestsFilename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        WsjcppLog::err(TAG, "save: could not open file to write: " + m_sDataTestsFilename);
        return false;
    }

    for (int i = 0; i < m_nDataTestsSize; i++) {
        file << m_vDataTests[i].getIn() << " " << m_vDataTests[i].getOut() << "\n";
    }
    file.close();
    return true;
}

void BNATestSin::calculateCurrentCounters() {
    m_pResults->resetCurrentCounters();
    for (int i = 0; i < m_vDataTests.size(); i++) {
        for (int x = 0; x < 32; x++) {
            BNABit bResult = m_pBNA->calc(m_vDataTests[i].getInOfBits(), x);
            if (m_vDataTests[i].getOutOfBits()[x] == bResult) {
                m_pResults->incrementCurrentCounter(x);
            }
        }
    }
    m_pResults->calcPercents(m_vDataTests.size());
    std::cout
        << m_vModificationModels[m_nCurrentModificationModel]->getMutationCicles() << ";"
        << m_vModificationModels[m_nCurrentModificationModel]->getAddCicles() << ";"
        << m_vModificationModels[m_nCurrentModificationModel]->getRemoveCicles() << ";"
        << m_pResults->getSummaryDiff() << ";"
        << std::endl;
}
