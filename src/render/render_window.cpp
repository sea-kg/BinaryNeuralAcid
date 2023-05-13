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

void RenderBNANode::setNode(BinaryNeuralAcidGraphNode *pNode, BNAGroup<BinaryNeuralAcidBit> *pBna) {
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

void RenderBNANode::setNodeOutput(BNANodeOutput *pNode, BNAGroup<BinaryNeuralAcidBit> *pBna) {
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
