
#include "bna_test_sin1.h"
#include <wsjcpp_core.h>

// -----------------------------------------------------------------
// BNATestSin1Item

BNATestSin1Item::BNATestSin1Item(float nIn, float nOut) {
    m_nIn = nIn;
    m_nOut = nOut;
    floatToBNABits(m_nIn, m_vIn);
    floatToBNABits(m_nOut, m_vOut);
}

float BNATestSin1Item::getIn() {
    return m_nIn;
}

float BNATestSin1Item::getOut() {
    return m_nOut;
}

const std::vector<BNABit> &BNATestSin1Item::getInOfBits() {
    return m_vIn;
}

const std::vector<BNABit> &BNATestSin1Item::getOutOfBits() {
    return m_vOut;
}

void BNATestSin1Item::floatToBNABits(const float &f, std::vector<BNABit> &vResult) {
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
// BNATestSin1

BNATestSin1::BNATestSin1() {
    TAG = "BNATestSin1";

    m_nSizeInput = 32; // input bits 
    m_nSizeOutput = 1; // output bits
    m_sBNAFilename = "testsin1";
    m_pBNA = new BNA(m_nSizeInput, m_nSizeOutput);
    m_sDataTestsFilename = "testsin1.bnadatatest";
    m_nDataTestsSize = 2000;
    m_pResults = new BNAStatCalcResults(1);

    m_vModificationModels.push_back(new BNAModificationModel(rand() % 1000, rand() % 1000, rand() % 1000));
    m_vModificationModels.push_back(new BNAModificationModel(1,0,0));
    m_vModificationModels.push_back(new BNAModificationModel(0,10,0));
    m_vModificationModels.push_back(new BNAModificationModel(0,0,1));
    m_nCurrentModificationModel = 0;
    m_nModificationModelCounter = 0;
}

bool BNATestSin1::run() {
    RenderBNA render(this);
    return render.run("bna - test-sin1");
}

bool BNATestSin1::onStart() {
    // 1. load last bna (or generate randomly)
    if (WsjcppCore::fileExists(m_sBNAFilename + ".bna")) {
        if (!m_pBNA->load(m_sBNAFilename)) {
            std::cout << "ERROR Could not load file " << m_sBNAFilename << ".bna" << std::endl;
            return false;
        }
    } else {
        m_pBNA->randomGenerate(m_nSizeInput, m_nSizeOutput, 1000);
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
    std::cout << "m_pResults->getAllCurrentCountersPercents() == " << m_pResults->getAllCurrentCountersPercents() << "%;" << std::endl;
    // exit(1);
    return true;
}

void BNATestSin1::doMutation() {
    m_nModificationModelCounter++;
    if (m_nModificationModelCounter > 100) {
        m_nModificationModelCounter = 0;
        m_nCurrentModificationModel += 1;
        m_nCurrentModificationModel = m_nCurrentModificationModel % m_vModificationModels.size();
        if (m_nCurrentModificationModel == 0) {
            m_vModificationModels[0]->update(rand() % 1000, rand() % 1000, rand() % 1000);
        }
    }
    m_pBNA->randomModify(m_vModificationModels[m_nCurrentModificationModel]);
}

void BNATestSin1::doTestAndRevert() {
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

BNA* BNATestSin1::getBNA() {
    return m_pBNA;
}

const BNAStatCalcResults *BNATestSin1::getResults() {
    return m_pResults;
}

void BNATestSin1::byteArrayToFloat(const unsigned char *pBytes, float &nResult) {
    std::memcpy(&nResult, pBytes, sizeof(nResult));
}

void BNATestSin1::print(unsigned char *pResult4) {
    for (std::size_t i = 0; i != 4; ++i) {
        std::printf("The byte #%zu is 0x%02X\n", i, pResult4[i]);
    }
}

void BNATestSin1::print(BNABit pResult[1]) {
    for (int i = 0; i < m_nSizeOutput; i++) {
        std::cout << (pResult[i] == BNABit::B_0 ? "0" : "1");
        if ( (i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void BNATestSin1::regenDataTests() {
    m_vDataTests.clear();
    
    float HI = 1.0f;
    float LO = -1.0f;
    for (int i = 0; i < m_nDataTestsSize; i++) {
        float r3 = LO + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(HI - LO)));
        float sin_r3 = sin(r3);
        m_vDataTests.push_back(BNATestSin1Item(r3, sin_r3));
    }
}

bool BNATestSin1::loadDataTests() {

    std::string sFilename = m_sDataTestsFilename;
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
        m_vDataTests.push_back(BNATestSin1Item(nIn, nOut));
    }
    return true;
}

bool BNATestSin1::saveDataTests() {
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

void BNATestSin1::calculateCurrentCounters() {
    m_pResults->resetCurrentCounters();
    for (int i = 0; i < m_vDataTests.size(); i++) {
        int x = 0;
        // for (int x = 0; x < m_nSizeOutput; x++) {
            BNABit bResult = m_pBNA->calc(m_vDataTests[i].getInOfBits(), x);
            if (m_vDataTests[i].getOutOfBits()[x] == bResult) {
                m_pResults->incrementCurrentCounter(x);
            }
        // }
    }
    m_pResults->calcPercents(m_vDataTests.size());
    // std::cout
    //     << m_vModificationModels[m_nCurrentModificationModel]->getMutationCicles() << ";"
    //     << m_vModificationModels[m_nCurrentModificationModel]->getAddCicles() << ";"
    //     << m_vModificationModels[m_nCurrentModificationModel]->getRemoveCicles() << ";"
    //     << std::endl << "m_pResults->getAllCurrentCountersPercents() == " << m_pResults->getAllCurrentCountersPercents() << "%;"
    //     << std::endl << "m_pResults->getSummaryDiff() == " << m_pResults->getSummaryDiff() << ";"
    //     << std::endl;
    // exit(1);
}
