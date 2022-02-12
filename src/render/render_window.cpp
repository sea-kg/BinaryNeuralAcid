#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <wsjcpp_core.h>

#include "render_window.h"

// -----------------------------------------------------------------
// RenderWindow

RenderWindow::RenderWindow(const char* title, int w, int h) {
    window = NULL;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
    }

    renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

RenderWindow::~RenderWindow() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
}

void RenderWindow::addObject(RenderObject *pObject) {
    m_vObjects.push_back(pObject);
}

void RenderWindow::removeObject(RenderObject *pObject) {
    auto it = std::find(m_vObjects.begin(), m_vObjects.end(), pObject);
    if (it != m_vObjects.end()) {
        m_vObjects.erase(it);
    }
}

void RenderWindow::sortObjectsByPositionZ() {
    int nSize = m_vObjects.size();
    if (nSize < 2) {
        return; // nothing to sort
    }
    int nPermutations = 1; // 1 for start
    while (nPermutations > 0) {
        nPermutations = 0;
        for (int i = 0; i < nSize - 2; i++) {
            if (m_vObjects[i]->getPositionZ() > m_vObjects[i+1]->getPositionZ()) {
                RenderObject *pObject = m_vObjects[i+1];
                m_vObjects[i+1] = m_vObjects[i];
                m_vObjects[i] = pObject;
                nPermutations++;
            }
        }
    }
}

void RenderWindow::cleanUp() {
    SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    // background color
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 125);
    // alpha
    SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = 1280;
    srcrect.h = 720;
    // m_color.changeRenderColor(renderer);
    SDL_RenderFillRect(renderer, &srcrect);

    // SDL_RenderClear(renderer);
}

void RenderWindow::modifyObjects(const AppState& state) {
    for (auto pObj: m_vObjects) {
        pObj->modify(state);
    }
}

void RenderWindow::drawObjects() {
    for (auto pObj: m_vObjects) {
        pObj->draw(renderer);
    }

    // finish
    SDL_RenderPresent(renderer);
}

// -----------------------------------------------------------------

RenderFpsCounter::RenderFpsCounter() {
    m_nEvery = 3000;
    this->reset();
}

void RenderFpsCounter::reset() {
    m_nNumberOfFrames = 0;
    m_nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    m_nElapsed = 0;
    m_nFps = 0;
}

bool RenderFpsCounter::addFrame() {
    m_nNumberOfFrames++;
    m_nElapsed = WsjcppCore::getCurrentTimeInMilliseconds() - m_nStartTime;
    if (m_nElapsed > m_nEvery) {
        double nFPS = m_nNumberOfFrames;
        nFPS /= m_nElapsed;
        nFPS *= 1000;
        m_nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
        m_nNumberOfFrames = 0;
        m_nFps = nFPS;
        return true;
    }
    return false;
}

int RenderFpsCounter::getFps() {
    return m_nFps;
}

// -----------------------------------------------------------------
// RenderBNANode

RenderBNANode::RenderBNANode(int nSizeNode)
    : RenderRect(
        CoordXY(100, 100),
        nSizeNode, nSizeNode,
        RenderColor(255,255,255,255),
        10 // z
    )
{
    m_nSizeNode = nSizeNode;
    m_pNodeInput = nullptr;
    m_pNode = nullptr;
    m_pNodeOutput = nullptr;
    m_nLevelY = -1;

    m_pColorNodeOutput = new RenderColor(255,255,255,255); // white
    m_pColorOperAnd = new RenderColor(179,244,8,255); // yellow
    m_pColorOperOr = new RenderColor(53,155,61,255); // green
    m_pColorOperXor = new RenderColor(213,25,25,255); // red
    m_pColorOperNxor = new RenderColor(77,79,216,255); // blue
};

void RenderBNANode::setNodeInput(BNANodeInput *pNode) {
    m_pNodeInput = pNode;
    m_pNode = nullptr;
    m_pNodeOutput = nullptr;
    this->updateColor(*m_pColorNodeOutput);
    m_nLevelY = 1;
}

void RenderBNANode::setNode(BNANode *pNode, BNA *pBna) {
    m_pNodeInput = nullptr;
    m_pNode = pNode;
    m_pNodeOutput = nullptr;

    const std::string &sOperType = m_pNode->getOperationType();

    if (sOperType == "AND") {
        this->updateColor(*m_pColorOperAnd);
    } else if (sOperType == "OR") {
        this->updateColor(*m_pColorOperOr);
    } else if (sOperType == "XOR") {
        this->updateColor(*m_pColorOperXor);
    } else if (sOperType == "NXOR") {
        this->updateColor(*m_pColorOperNxor);
    } else {
        exit(1);
    }
    m_nLevelY = std::max(
        pBna->calculateDepth(m_pNode->getX()),
        pBna->calculateDepth(m_pNode->getY())
    );
//    std::cout << "m_nLevelY=" << m_nLevelY << std::endl;
}

void RenderBNANode::setNodeOutput(BNANodeOutput *pNode, BNA *pBna) {
    m_pNodeInput = nullptr;
    m_pNode = nullptr;
    m_pNodeOutput = pNode;
    this->updateColor(*m_pColorNodeOutput);
    short n = m_pNodeOutput->getInputNodeIndex();
    m_nLevelY = pBna->calculateDepth(n);
}

int RenderBNANode::getLevelY() {
    return m_nLevelY;
}

// -----------------------------------------------------------------
// RenderBNA

RenderBNA::RenderBNA(ICallbacksRenderBNA *pCallbakcs) {
    TAG = "RenderBNA";
    m_pCallbacksRenderBNA = pCallbakcs;
    m_nWindowWidth = 1280;
    m_nWindowHeight = 720;
    m_nSizeNode = 6;
    m_nPadding = 100;
    m_vRenderPrevOutputResult = nullptr;
    m_vRenderCurrentOutputResult = nullptr;
}

bool RenderBNA::run(const std::string &sWindowName) {

    m_pCallbacksRenderBNA->onStart();

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cerr << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return -1;
    }

    // init window
    CoordXY centerPoint(m_nWindowWidth/2, m_nWindowHeight/2);
    AppState appState(m_nWindowWidth, m_nWindowHeight);
    m_pWindow = new RenderWindow(
        sWindowName.c_str(),
        appState.windowWidth(),
        appState.windowHeight()
    );
    this->createFpsText();
    bool appRunning = true;

    SDL_Event event;
    m_fps.reset();
    appState.init();

    while (appRunning) {
        
        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    appRunning = false;
                    break;
                default:
                    break; // nothing
            }
        }
        // TODO RenderNodes and connections

        // // draw current 
        this->prepareNodes();
        appState.updateElapsedTime();
        m_pWindow->clear();
        m_pWindow->modifyObjects(appState);
        m_pWindow->drawObjects();

        // // draw mutations
        m_pCallbacksRenderBNA->doMutation();
        this->prepareNodes();
        m_pWindow->clear();
        m_pWindow->modifyObjects(appState);
        m_pWindow->drawObjects();

        m_pCallbacksRenderBNA->doTestAndRevert();

        // FPS
        if (m_fps.addFrame()) {
            m_pFpsText->updateText("FPS: " + std::to_string(m_fps.getFps()));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    m_pWindow->cleanUp();
    SDL_Quit();
    return false;
}

void RenderBNA::createFpsText() {
    m_pFpsText = new RenderAbsoluteTextBlock(CoordXY(50,20), "FPS: ----", 1000);
    m_pWindow->addObject(m_pFpsText);
    m_pWindow->sortObjectsByPositionZ();
}

void RenderBNA::prepareVectorsSize() {
    BNA *pBna = m_pCallbacksRenderBNA->getBNA();
    int nInputSize = pBna->getInputSize();
    int nNodesSize = pBna->getNodesSize();
    int nOutputSize = pBna->getOutputSize();

    // CONNECTIONS
    int nConnectionsCount = nNodesSize*2 + nOutputSize;
    while (m_vRenderConnections.size() < nConnectionsCount) {
        // add new connections
        RenderConnection *pLine = new RenderConnection(
            CoordXY(100, 100),
            CoordXY(200, 200),
            RenderColor(255,255,255,255)
        );
        m_vRenderConnections.push_back(pLine);
        m_pWindow->addObject(pLine);
    }

    while (m_vRenderConnections.size() > nConnectionsCount) {
        // remove extra conenctions
        RenderConnection *pLine = m_vRenderConnections.back();
        m_vRenderConnections.pop_back();
        m_pWindow->removeObject(pLine);
        delete pLine;
    }

    prepareRendorBNANodes();

    // OUTPUT TEXT
    int nOutputWidth = m_nWindowWidth - m_nPadding*2;
    if (nOutputSize > 1) {
        nOutputWidth = nOutputWidth / (nOutputSize - 1);
    } else {
        nOutputWidth /= 2;
    }
    
    while (m_vRenderCurrentOutputsResult.size() < nOutputSize) {
        int i = m_vRenderCurrentOutputsResult.size();
        RenderAbsoluteTextBlock * pText = new RenderAbsoluteTextBlock(
            CoordXY(
                m_nPadding + i * nOutputWidth,
                m_nWindowHeight - m_nPadding + 20
            ), "."
        );
        m_vRenderCurrentOutputsResult.push_back(pText);
        m_pWindow->addObject(pText);
    }

    if (m_vRenderCurrentOutputResult == nullptr) {
        m_vRenderCurrentOutputResult = new RenderAbsoluteTextBlock(
            CoordXY(m_nWindowWidth/2, m_nWindowHeight - m_nPadding + 35), "result"
        );
        m_pWindow->addObject(m_vRenderCurrentOutputResult);
    }
    while (m_vRenderPrevOutputsResult.size() < nOutputSize) {
        int i = m_vRenderPrevOutputsResult.size();
        RenderAbsoluteTextBlock * pText = new RenderAbsoluteTextBlock(
            CoordXY(
                m_nPadding + i * nOutputWidth,
                m_nWindowHeight - m_nPadding + 60
            ), "."
        );
        m_vRenderPrevOutputsResult.push_back(pText);
        m_pWindow->addObject(pText);
    }
    if (m_vRenderPrevOutputResult == nullptr) {
        m_vRenderPrevOutputResult = new RenderAbsoluteTextBlock(
            CoordXY(m_nWindowWidth/2, m_nWindowHeight - m_nPadding + 75), "result"
        );
        m_pWindow->addObject(m_vRenderPrevOutputResult);
    }
}

void RenderBNA::prepareRendorBNANodes() {
    BNA *pBna = m_pCallbacksRenderBNA->getBNA();
    int nInputSize = pBna->getInputSize();
    int nNodesSize = pBna->getNodesSize();
    int nOutputSize = pBna->getOutputSize();

    // NODES
    int nAllCountNodes = nNodesSize + nInputSize + nOutputSize;
    
    // add missing nodes
    while (m_vRenderNodes.size() < nAllCountNodes) {
        RenderBNANode *pRect = new RenderBNANode(m_nSizeNode);
        m_vRenderNodes.push_back(pRect);
        m_pWindow->addObject(pRect);
    }
    // remove extra nodes
    while (m_vRenderNodes.size() > nAllCountNodes) {
        RenderBNANode *pRect = m_vRenderNodes.back();
        m_vRenderNodes.pop_back();
        m_pWindow->removeObject(pRect);
        delete pRect;
    }

    // SET NODES
    const std::vector<BNANodeInput *> &vInputNodes = pBna->getNodesInput();
    for (int i = 0; i < vInputNodes.size(); i++) {
        m_vRenderNodes[i]->setNodeInput(vInputNodes[i]);
    }
    const std::vector<BNANode *> &vNodes = pBna->getNodes();
    for (int i = 0; i < vNodes.size(); i++) {
        m_vRenderNodes[nInputSize + i]->setNode(vNodes[i], pBna);
    }
    const std::vector<BNANodeOutput *> &vNodesOutput = pBna->getNodesOutput();
    for (int i = 0; i < vNodesOutput.size(); i++) {
        m_vRenderNodes[nInputSize + nNodesSize + i]->setNodeOutput(vNodesOutput[i], pBna);
    }
}

int RenderBNA::recurciveCalculateYLevel(int nInputCount, const std::vector<BNANode *> &vItems, BNANode *pItem, int nCounter) {
    if (pItem->getX() < nInputCount || pItem->getY() < nInputCount) {
        return nCounter + 1;
    }
    BNANode *pItemLeft = vItems[pItem->getX() - nInputCount];
    BNANode *pItemRight = vItems[pItem->getY() - nInputCount];
    int nLeft = recurciveCalculateYLevel(nInputCount, vItems, pItemLeft, nCounter + 1);
    int nRight = recurciveCalculateYLevel(nInputCount, vItems, pItemRight, nCounter + 1);
    return std::max(nLeft, nRight);
}

void RenderBNA::updateInputNodesXY() {
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getNodes();

    int nInputCount = m_pCallbacksRenderBNA->getBNA()->getInputSize();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->getOutputSize();

    int nStartIndex = 0;
    int nEndIndex = 1;

    int nInputWidth = m_nWindowWidth - m_nPadding*2;
    nInputWidth = nInputWidth / (nInputCount - 1);
    nStartIndex = 0;
    nEndIndex = nInputCount - 1;
    for (int i = nStartIndex; i <= nEndIndex; i++) {
        m_vRenderNodes[i]->updateXY(
            m_nPadding + i * nInputWidth - m_nSizeNode,
            m_nPadding
        );
    }
}

std::vector<RenderBNANode *> RenderBNA::getChildAndParantNodes(int nIndex) {
    std::vector<RenderBNANode *> vRet;
    const std::vector<BNANode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->getInputSize();
    int nCurrentIndex = nIndex - nInputCount;
    int nXIndex = vNodes[nCurrentIndex]->getX();
    int nYIndex = vNodes[nCurrentIndex]->getY();
    vRet.push_back(m_vRenderNodes[nXIndex]);
    vRet.push_back(m_vRenderNodes[nYIndex]);

    for (int i = 0; i < vNodes.size(); i++) {
        if (vNodes[i]->getX() == nIndex || vNodes[i]->getY() == nIndex) {
            vRet.push_back(m_vRenderNodes[i + nInputCount]);
        }
    }

    const std::vector<BNANodeOutput *> &vNodesOutput = m_pCallbacksRenderBNA->getBNA()->getNodesOutput();
    for (int i = 0; i < vNodesOutput.size(); i++) {
        if (vNodesOutput[i]->getInputNodeIndex() == nIndex) {
            vRet.push_back(m_vRenderNodes[m_vRenderNodes.size() - vNodesOutput.size() + i]);
        }
    }
    return vRet;
}

std::vector<RenderBNANode *> RenderBNA::getParentNodes(int nIndex) {
    std::vector<RenderBNANode *> vRet;
    const std::vector<BNANode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->getInputSize();
    int nCurrentIndex = nIndex - nInputCount;
    int nXIndex = vNodes[nCurrentIndex]->getX();
    int nYIndex = vNodes[nCurrentIndex]->getY();
    vRet.push_back(m_vRenderNodes[nXIndex]);
    vRet.push_back(m_vRenderNodes[nYIndex]);

    return vRet;
}

std::vector<RenderBNANode *> RenderBNA::getChildNodes(int nIndex) {
    std::vector<RenderBNANode *> vRet;
    const std::vector<BNANode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->getInputSize();
    int nCurrentIndex = nIndex - nInputCount;
    
    for (int i = 0; i < vNodes.size(); i++) {
        if (vNodes[i]->getX() == nIndex || vNodes[i]->getY() == nIndex) {
            vRet.push_back(m_vRenderNodes[i + nInputCount - 1]);
        }
    }

    const std::vector<BNANodeOutput *> &vNodesOutput = m_pCallbacksRenderBNA->getBNA()->getNodesOutput();
    for (int i = 0; i < vNodesOutput.size(); i++) {
        if (vNodesOutput[i]->getInputNodeIndex() == nIndex) {
            vRet.push_back(m_vRenderNodes[m_vRenderNodes.size() - vNodesOutput.size() + i]);
        }
    }

    return vRet;
}

int RenderBNA::distance(const CoordXY &p0, const CoordXY &p1) {
    int nRet = sqrt((p1.x()-p0.x())*(p1.x()-p0.x())+(p1.y()-p0.y())*(p1.y()-p0.y()));
    return nRet;
}

int RenderBNA::distanceN(int n0, int n1) {
    int _n0 = std::min(n0,n1);
    int _n1 = std::max(n0,n1);
    return _n1 - _n0;
}

void RenderBNA::updateMiddleNodesXY2() {
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getNodes();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->getInputSize();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->getOutputSize();
    int nStartIndex = nInputCount;
    int nEndIndex = m_vRenderNodes.size() - nOutputCount - 1;

    int nMaxLevel = 0;
    for (int i = 0; i < m_vRenderNodes.size(); i++) {
        nMaxLevel = std::max(nMaxLevel, m_vRenderNodes[i]->getLevelY());
    }
    // std::cout << "nMaxLevel = " << nMaxLevel << std::endl;
    
    int nLevelHeight = (m_nWindowHeight - m_nPadding*3) / nMaxLevel;

    for (int i = nStartIndex; i <= nEndIndex; i++) {
        std::vector<RenderBNANode *> vChildNodes = getChildNodes(i);
        std::vector<RenderBNANode *> vParentNodes = getParentNodes(i);
        std::vector<RenderBNANode *> vNodes = getChildAndParantNodes(i);

        int nMinY = vChildNodes.size() > 0 ? vChildNodes[0]->getCoord().y() : 100;
        for (int n = 0; n < vChildNodes.size(); n++) {
            nMinY = std::max(nMinY, vChildNodes[n]->getCoord().y());
        }

        int nMaxY = vParentNodes.size() > 0 ? vParentNodes[0]->getCoord().y() : 100;
        for (int n = 0; n < vParentNodes.size(); n++) {
            nMaxY = std::min(nMaxY, vParentNodes[n]->getCoord().y());
        }

        int nMinX = m_nWindowWidth;
        int nMaxX = 0;
        if (vNodes.size() > 0) {
            nMinX = vNodes[0]->getCoord().x();
            nMaxX = vNodes[0]->getCoord().x();
        }
        
        for (int n = 0; n < vNodes.size(); n++) {
            nMinX = std::min(nMinX, vNodes[n]->getCoord().x());
            nMaxX = std::max(nMaxX, vNodes[n]->getCoord().x());
        }
        int dX = nMaxX - nMinX;
        int dY = nMaxY - nMinY;

        int nX = nMinX + dX / 2;
        int nY = nMinY + dY / 2;

        if (nY < nMinY) {
            nY = nMinY;
        }
        m_vRenderNodes[i]->updateXY(
            nX,
            m_nPadding + m_nPadding/2 + nLevelHeight * m_vRenderNodes[i]->getLevelY() // nY
        );
    }

    bool bDistance = true;
    while (bDistance) {
        bDistance = false;
        for (int i0 = nStartIndex; i0 <= nEndIndex; i0++) {
            for (int i1 = nStartIndex; i1 <= nEndIndex; i1++) {
                if (i0 != i1) {
                    const CoordXY &p0 = m_vRenderNodes[i0]->getCoord();
                    const CoordXY &p1 = m_vRenderNodes[i1]->getCoord();
                    // if (p0.x() == p1.x() && p0.y() == p1.y()) {
                    //     m_vRenderNodes[i0]->updateXY(p0.x() - 20, p0.y() + 10);
                    //     m_vRenderNodes[i1]->updateXY(p1.x() + 20, p1.y() - 10);
                    // }
                    // int nDistance = distance(p0, p1);
                    // if (nDistance < 20) {
                    //     bDistance = true;
                    //     // std::cout << "nDistance = " << nDistance << std::endl;
                    //     if (distanceN(p0.x(), p1.x()) < 30) {
                    //         m_vRenderNodes[i0]->updateXY(p0.x() - 20, p0.y());
                    //         m_vRenderNodes[i1]->updateXY(p1.x() + 20, p1.y());
                    //     }
                    //     if (distanceN(p0.y(), p1.y()) < 20) {
                    //         int nDiff = 40;
                    //         if (p0.y() - nDiff < 100) {
                    //             nDiff = p0.y() + 20;
                    //         }
                    //         //m_vRenderNodes[i0]->updateXY(p0.x(), p0.y());
                    //         // m_vRenderNodes[i1]->updateXY(p1.x(), p1.y() + nDiff);
                    //     }
                    //     else {
                    //         m_vRenderNodes[i0]->updateXY(p0.x() - 20, p0.y() + 20);
                    //         m_vRenderNodes[i1]->updateXY(p1.x() + 20, p1.y() - 20);
                    //     }
                    // }
                }
            }
        }
    }
}

void RenderBNA::updateOutputNodesXY() {
    const std::vector<BNANodeOutput *> &vNodesOutput = m_pCallbacksRenderBNA->getBNA()->getNodesOutput();
    // int nOutputSize = m_pCallbacksRenderBNA->getBNA()->getOutputSize();

    // output nodes
    int nOutputWidth = m_nWindowWidth - m_nPadding*2;
    if (vNodesOutput.size() > 1) {
        nOutputWidth = nOutputWidth / (vNodesOutput.size() - 1);
    } else {
        nOutputWidth = nOutputWidth / 2;
    }
    
    for (int i = 0; i < vNodesOutput.size(); i++) {
        int nIndex = m_vRenderNodes.size() - vNodesOutput.size() + i; 
        m_vRenderNodes[nIndex]->updateXY(
            m_nPadding + i * nOutputWidth,
            m_nWindowHeight - m_nPadding
        );
    }

    // update current results
    const BNAStatCalcResults *pResults = m_pCallbacksRenderBNA->getResults();
    const std::vector<int> &vPrevCountersPercents = pResults->getPrevCountersPercents();
    const std::vector<int> &vCurrentCountersPercents = pResults->getCurrentCountersPercents();

    std::string sNewText = "";
    for (int i = 0; i < vPrevCountersPercents.size(); i++) {
        sNewText = std::to_string(vPrevCountersPercents[i]) + "%";
        m_vRenderPrevOutputsResult[i]->updateText(sNewText);
    }
    sNewText = std::to_string(pResults->getAllPrevCountersPercents()) + "%";
    m_vRenderPrevOutputResult->updateText(sNewText);
    for (int i = 0; i < vCurrentCountersPercents.size(); i++) {
        sNewText = std::to_string(vCurrentCountersPercents[i]) + "%";
        m_vRenderCurrentOutputsResult[i]->updateText(sNewText);
    }
    sNewText = std::to_string(pResults->getAllCurrentCountersPercents()) + "%";
    m_vRenderCurrentOutputResult->updateText(sNewText);
}

void RenderBNA::updateNodesConnections() {
    int nInputSize = m_pCallbacksRenderBNA->getBNA()->getInputSize();
    const std::vector<BNANode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
    const std::vector<BNANodeOutput *> &vNodesOutput = m_pCallbacksRenderBNA->getBNA()->getNodesOutput();

    // connections
    for (int i = 0; i < vNodes.size(); i++) {
        RenderRect *pCurrent = m_vRenderNodes[i + nInputSize];
        int nInX = vNodes[i]->getX();
        if (nInX >= m_vRenderNodes.size()) {
            WsjcppLog::throw_err(TAG, "nInX >= m_vRenderNodes.size()");
        }
        RenderRect *pLeft = m_vRenderNodes[nInX];
        updateLine(i*2, pLeft, pCurrent);
        if (vNodes[i]->getY() >= m_vRenderNodes.size()) {
            WsjcppLog::throw_err(TAG, "vNodes[i]->getY() >= m_vRenderNodes.size()");
        }
        RenderRect *pRight = m_vRenderNodes[vNodes[i]->getY()];
        updateLine(i*2 + 1, pRight, pCurrent);
    }

    for (int i = 0; i < vNodesOutput.size(); i++) {
        int nOutputRectIndex = m_vRenderNodes.size() - vNodesOutput.size() + i;
        if (nOutputRectIndex >= m_vRenderNodes.size()) {
            WsjcppLog::throw_err(TAG, "nOutputRectIndex >= m_vRenderNodes.size()");
        }
        RenderRect *pCurrent = m_vRenderNodes[nOutputRectIndex];
        int nIndexLine = vNodesOutput[i]->getInputNodeIndex();
        RenderRect *pFrom = m_vRenderNodes[nIndexLine];
        if (nIndexLine >= m_vRenderNodes.size()) {
            WsjcppLog::throw_err(TAG, "nIndexLine >= m_vRenderNodes.size()");
        }
        updateLine(vNodes.size()*2 + i, pFrom, pCurrent);
    }
}

void RenderBNA::prepareNodes() {
    
    this->prepareVectorsSize();
    this->updateInputNodesXY();
    this->updateMiddleNodesXY2();
    this->updateOutputNodesXY();
    this->updateNodesConnections();
}

void RenderBNA::updateLine(int nIndexLine, RenderRect *pRect0, RenderRect *pRect1) {
    CoordXY p1 = pRect0->getCoord() + CoordXY(m_nSizeNode/2, m_nSizeNode/2);
    CoordXY p2 = pRect1->getCoord() + CoordXY(m_nSizeNode/2, m_nSizeNode/2);
    if (nIndexLine >= m_vRenderConnections.size()) {
        WsjcppLog::throw_err(TAG, "nIndexLine >= m_vRenderConnections.size()");
    }
    m_vRenderConnections[nIndexLine]->updateCoords(p1, p2);
    m_vRenderConnections[nIndexLine]->updateColor(pRect1->getColor());
    // return nIndexLine + 1;
}
