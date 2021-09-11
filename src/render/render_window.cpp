#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
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
// RenderBNA

RenderBNA::RenderBNA(ICallbacksRenderBNA *pCallbakcs) {
    m_pCallbacksRenderBNA = pCallbakcs;
    m_nWindowWidth = 1280;
    m_nWindowHeight = 720;
    m_nSizeNode = 6;
    m_nPadding = 100;
    m_pColorOperAnd = new RenderColor(179,244,8,255); // yellow
    m_pColorOperOr = new RenderColor(53,155,61,255); // green
    m_pColorOperXor = new RenderColor(213,25,25,255); // red
    m_pColorOperNxor = new RenderColor(77,79,216,255); // blue
    m_vRenderOutputResult = nullptr;
}

bool RenderBNA::run() {
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
        "bna - test-sin",
        appState.windowWidth(),
        appState.windowHeight()
    );
    RenderAbsoluteTextBlock *pFpsText = new RenderAbsoluteTextBlock(CoordXY(50,20), "FPS: ----", 1000);
    m_pWindow->addObject(pFpsText);
    m_pWindow->sortObjectsByPositionZ();
    bool appRunning = true;

    SDL_Event event;
    long nNumberOfFrames = 0;
    long nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    long nElapsed = 0;
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
        prepareNodes();
        appState.updateElapsedTime();
        m_pWindow->clear();
        m_pWindow->modifyObjects(appState);
        m_pWindow->drawObjects();

        // // draw mutations
        m_pCallbacksRenderBNA->doMutation();
        prepareNodes();
        m_pWindow->clear();
        m_pWindow->modifyObjects(appState);
        m_pWindow->drawObjects();

        // TODO uncomment
        m_pCallbacksRenderBNA->doTestAndRevert();

        // FPS
        nNumberOfFrames++;
        nElapsed = WsjcppCore::getCurrentTimeInMilliseconds() - nStartTime;
        if (nElapsed > 3000) {
            double nFPS = nNumberOfFrames;
            nFPS /= nElapsed;
            nFPS *= 1000;
            std::cout << "FPS: " << nFPS << std::endl;
            nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
            nNumberOfFrames = 0;
            pFpsText->updateText("FPS: " + std::to_string(int(nFPS)));
        }
    }
    m_pWindow->cleanUp();
    SDL_Quit();
    return false;
}

void RenderBNA::prepareVectorsSize() {
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->inputCount();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->outputCount();

    // CONNECTIONS
    for (int i = 0; i < vItems.size()*2; i++) {
        if (m_vRenderConnections.size() < i + 1) {
            RenderConnection *pLine = new RenderConnection(
                CoordXY(100, 100),
                CoordXY(200, 200),
                RenderColor(255,255,255,255)
            );
            m_vRenderConnections.push_back(pLine);
            m_pWindow->addObject(pLine);
        }
    }
    // std::cout << "m_vRenderConnections.size(): " << m_vRenderConnections.size() << std::endl;
    // TODO remove extra connections

    // NODES
    // if not exists
    int nAllCountNodes = vItems.size() + nInputCount;
    for (int i = 0; i < nAllCountNodes; i++) {
        if (m_vRenderNodes.size() < i + 1) {
            RenderRect *pRect = new RenderRect(
                CoordXY(100, 100),
                m_nSizeNode, // width
                m_nSizeNode, // hwight
                RenderColor(255,255,255,255), 
                10 // z
            );
            m_vRenderNodes.push_back(pRect);
            m_pWindow->addObject(pRect);
        }
    }
    // std::cout << "m_vRenderNodes.size(): " << m_vRenderNodes.size() << std::endl;

    // TODO remove extra nodes

    for (int i = 0; i < vItems.size(); i++) {
        if (m_vNodesPositions.size() < i + 1) {
            m_vNodesPositions.push_back(BNAItemPosition());
        }
    }
    // std::cout << "m_vNodesPositions.size(): " << m_vNodesPositions.size() << std::endl;
    // TODO remove extra node positions


    // OUTPUT TEXT
    int nOutputWidth = m_nWindowWidth - m_nPadding*2;
    nOutputWidth = nOutputWidth / (nOutputCount - 1);
    for (int i = 0; i < nOutputCount; i++) {
        if (m_vRenderOutputsResult.size() < i + 1) {
            RenderAbsoluteTextBlock * pText = new RenderAbsoluteTextBlock(
                CoordXY(
                    m_nPadding + i * nOutputWidth,
                    m_nWindowHeight - m_nPadding + 20
                ), "."
            );
            m_vRenderOutputsResult.push_back(pText);
            m_pWindow->addObject(pText);
        }
    }
    if (m_vRenderOutputResult == nullptr) {
        m_vRenderOutputResult = new RenderAbsoluteTextBlock(
            CoordXY(m_nWindowWidth/2, m_nWindowHeight - m_nPadding + 40), "result"
        );
        m_pWindow->addObject(m_vRenderOutputResult);
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

void RenderBNA::prepareLevels() {
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->inputCount();

    // prepare Y levels
    m_maxYLevel = 0;
    for (int i = 0; i < vItems.size(); i++) {
        m_vNodesPositions[i].ylevel = recurciveCalculateYLevel(nInputCount, vItems, vItems[i], 0);
        m_maxYLevel = std::max(m_vNodesPositions[i].ylevel, m_maxYLevel);
    }

    // prepare X levels
    m_maxXLevels.resize(m_maxYLevel+1);
    for (int i = 0; i < m_maxXLevels.size(); i++) {
        m_maxXLevels[i] = 0;
    }

    for (int i = 0; i < m_vNodesPositions.size(); i++) {
        int nYLevel = m_vNodesPositions[i].ylevel;
        m_vNodesPositions[i].xlevel = m_maxXLevels[nYLevel];
        m_maxXLevels[nYLevel]++;
    }
}

void RenderBNA::updateInputNodesXY() {
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();

    int nInputCount = m_pCallbacksRenderBNA->getBNA()->inputCount();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->outputCount();

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

void RenderBNA::updateMiddleNodesXY() {
    prepareLevels();
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->inputCount();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->outputCount();
    int nYStep = m_nWindowHeight - m_nPadding*2;
    std::cout << "m_maxYLevel = " << m_maxYLevel << std::endl;
    nYStep = nYStep / (m_maxYLevel + 1);
    int nStartIndex = nInputCount;
    int nEndIndex = m_vRenderNodes.size() - nOutputCount - 1;
    for (int i = nStartIndex; i <= nEndIndex; i++) {
        int nXLevel = m_vNodesPositions[i - nInputCount].xlevel;
        int nYLevel = m_vNodesPositions[i - nInputCount].ylevel;
        
        int nXStep = m_nWindowWidth - m_nPadding*2;
        int nXLevelPadding = 0;
        int nMaxXLevels = m_maxXLevels[nYLevel] + 1;
        std::cout << "m_maxXLevels[" << nYLevel << "] = " << m_maxXLevels[nYLevel]  << std::endl;
        if (nMaxXLevels > 0) {
            nXStep = nXStep / nMaxXLevels;
        } else {
            nXStep = nXStep / 2;
        }
        // nXLevelPadding = nXStep / 2;

        m_vRenderNodes[i]->updateXY(
            m_nPadding + nXLevelPadding + (nXStep * (nXLevel+1)),
            m_nPadding + nYStep * nYLevel
        );
        updateColorNode(i, vItems[i - nInputCount]->getOperationType());
    }
}

std::vector<RenderRect *> RenderBNA::getChildAndParantNodes(int nIndex) {
    std::vector<RenderRect *> vRet;
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->inputCount();
    int nXIndex = vItems[nIndex - nInputCount]->getX();
    int nYIndex = vItems[nIndex - nInputCount]->getY();
    vRet.push_back(m_vRenderNodes[nXIndex]);
    vRet.push_back(m_vRenderNodes[nYIndex]);

    for (int i = 0; i < vItems.size(); i++) {
        if (vItems[i]->getX() == nIndex || vItems[i]->getY() == nIndex) {
            vRet.push_back(m_vRenderNodes[i + nInputCount]);
        }
    }
    return vRet;
}

void RenderBNA::updateMiddleNodesXY2() {
    prepareLevels();
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();
    int nInputCount = m_pCallbacksRenderBNA->getBNA()->inputCount();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->outputCount();
    int nYStep = m_nWindowHeight - m_nPadding*2;
    // std::cout << "m_maxYLevel = " << m_maxYLevel << std::endl;
    nYStep = nYStep / (m_maxYLevel + 1);
    int nStartIndex = nInputCount;
    int nEndIndex = m_vRenderNodes.size() - nOutputCount - 1;
    for (int i = nStartIndex; i <= nEndIndex; i++) {
        updateColorNode(i, vItems[i - nInputCount]->getOperationType());
    }

    for (int i = nStartIndex; i <= nEndIndex; i++) {
        std::vector<RenderRect *> vNodes = getChildAndParantNodes(i);
        int nX = 0;
        int nY = 0;
        for (int n = 0; n < vNodes.size(); n++) {
            nX += vNodes[n]->getCoord().x();
            nY += vNodes[n]->getCoord().y();
        }
        if (vNodes.size() > 1) {
            nX /= vNodes.size();
            nY /= vNodes.size();
        } else {
            nX += 10;
            nY += 10;
        }
        
        m_vRenderNodes[i]->updateXY(nX, nY);
    }

    for (int x = nStartIndex; x <= nEndIndex; x++) {
        for (int y = nStartIndex; y <= nEndIndex; y++) {
            if (x != y) {
                const CoordXY &pX = m_vRenderNodes[x]->getCoord();
                const CoordXY &pY = m_vRenderNodes[y]->getCoord();
                if (pX.x() == pY.x() && pX.y() == pY.y()) {
                    m_vRenderNodes[x]->updateXY(pX.x() - 20, pX.y() + 10);
                    m_vRenderNodes[y]->updateXY(pY.x() + 20, pY.y() - 10);
                }
            }
        }
    }
}

void RenderBNA::updateOutputNodesXY() {
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->outputCount();

    // output nodes
    int nOutputWidth = m_nWindowWidth - m_nPadding*2;
    nOutputWidth = nOutputWidth / (nOutputCount - 1);
    int nStartIndex = m_vRenderNodes.size() - nOutputCount;
    int nEndIndex = m_vRenderNodes.size() - 1;
    for (int i = nStartIndex; i <= nEndIndex; i++) {
        m_vRenderNodes[i]->updateXY(
            m_nPadding + (i - nStartIndex) * nOutputWidth,
            m_nWindowHeight - m_nPadding
        );
    }

    // update current results
    int nDataTestsSize = m_pCallbacksRenderBNA->getDataTestsSize();
    std::vector<int> &vCounters = m_pCallbacksRenderBNA->getPrevCounters();
    int nAll = 0;
    for (int i = 0; i < vCounters.size(); i++) {
        nAll += vCounters[i];
        int nPercent = vCounters[i]*100/nDataTestsSize;
        std::string sNewText = std::to_string(vCounters[i]*100/nDataTestsSize);
        m_vRenderOutputsResult[i]->updateText(sNewText + "%");
    }
    std::string sNewText = std::to_string(nAll*100/(nDataTestsSize*vCounters.size()));
    m_vRenderOutputResult->updateText(sNewText + "%");
}

void RenderBNA::updateNodesConnections() {
    const std::vector<BNANode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getItems();

    int nInputCount = m_pCallbacksRenderBNA->getBNA()->inputCount();
    int nOutputCount = m_pCallbacksRenderBNA->getBNA()->outputCount();

    // connections
    int nStartIndex = nInputCount;
    int nEndIndex = m_vRenderNodes.size() - 1;
    int nIndexLine = 0;
    for (int i = nStartIndex; i <= nEndIndex; i++) {
        RenderRect *pCurrent = m_vRenderNodes[i];
        int nInX = vItems[i - nInputCount]->getX();
        RenderRect *pLeft = m_vRenderNodes[nInX];
        nIndexLine = updateLine(nIndexLine, pLeft, pCurrent);
        int nInY = vItems[i - nInputCount]->getY();
        RenderRect *pRight = m_vRenderNodes[nInY];
        nIndexLine = updateLine(nIndexLine, pRight, pCurrent);
    }
}

void RenderBNA::prepareNodes() {
    prepareVectorsSize();

    updateInputNodesXY();

    // updateMiddleNodesXY();
    updateMiddleNodesXY2();

    updateOutputNodesXY();

    updateNodesConnections();
}

int RenderBNA::updateLine(int nIndexLine, RenderRect *pRect0, RenderRect *pRect1) {
    CoordXY p1 = pRect0->getCoord() + CoordXY(m_nSizeNode/2, m_nSizeNode/2);
    CoordXY p2 = pRect1->getCoord() + CoordXY(m_nSizeNode/2, m_nSizeNode/2);
    m_vRenderConnections[nIndexLine]->updateCoords(p1, p2);
    m_vRenderConnections[nIndexLine]->updateColor(pRect1->getColor());
    return nIndexLine + 1;
}

void RenderBNA::updateColorNode(int nIndexNode, const std::string &sOperType) {
    if (sOperType == "AND") {
        m_vRenderNodes[nIndexNode]->updateColor(*m_pColorOperAnd);
    } else if (sOperType == "OR") {
        m_vRenderNodes[nIndexNode]->updateColor(*m_pColorOperOr);
    } else if (sOperType == "XOR") {
        m_vRenderNodes[nIndexNode]->updateColor(*m_pColorOperXor);
    } else if (sOperType == "NXOR") {
        m_vRenderNodes[nIndexNode]->updateColor(*m_pColorOperNxor);
    } else {
        exit(1);
    }
}