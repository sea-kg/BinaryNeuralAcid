
#include "bna_test_sin.h"
#include <wsjcpp_core.h>
#include <iostream>
#include <cstdint>
#include <cstring>

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
    m_nDataTestsSize = 10000;

    // init counters
    m_prevCounters.resize(32);
    m_currentCounters.resize(32);
    for (int i = 0; i < 32; i++) {
        m_prevCounters[i] = 0;
        m_currentCounters[i] = 0;
    }
}

bool BNATestSin::run() {
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
    m_prevCounters = m_currentCounters;

    int nCicles = 0;
    int nMaxCicles = 1000;
    while (nCicles < nMaxCicles) {
        nCicles++;

        // modify 
        int nModifyCicles = rand() % 15 + 2;
        std::cout << "ModifyCicles " << nModifyCicles << std::endl;
        m_pBNA->generateRandomMutations(nModifyCicles);
        calculateCurrentCounters();
        int nDiff = printCounters();
        if (nDiff > 0) {
            // oo! nice current calculation better
            m_pBNA->save(m_sBNAFilename);
            m_prevCounters = m_currentCounters;
        } else {
            std::cout << "Reset bna " << std::endl;
            m_pBNA->load(m_sBNAFilename);
        }
    }
    

    // BNABit calc(const std::vector<BNABit> &vInputs, int nOutput);
    // 3. run check 
    // 4. run modifications
    // 5. run check
    // 6. save or reload 
    // 7. go to 4
    return true;
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

int BNATestSin::printCounters() {
    int nSummaryDiff = 0;
    int nDiff;
    for (int i = 0; i < 32; i++) {
        nDiff = m_currentCounters[i] - m_prevCounters[i];
        nSummaryDiff += nDiff;
        std::cout << i << ": " << m_prevCounters[i] << " -> " << m_currentCounters[i] << " diff: " << nDiff << std::endl;
    }
    std::cout << "summary diff: " << nSummaryDiff << std::endl;
    return nSummaryDiff;
}

void BNATestSin::calculateCurrentCounters() {
    for (int i = 0; i < 32; i++) {
        m_currentCounters[i] = 0;
    }
    for (int i = 0; i < m_vDataTests.size(); i++) {
        for (int x = 0; x < 32; x++) {
            BNABit bResult = m_pBNA->calc(m_vDataTests[i].getInOfBits(), x);
            if (m_vDataTests[i].getOutOfBits()[x] == bResult) {
                m_currentCounters[x]++;
            }
        }
    }
}
