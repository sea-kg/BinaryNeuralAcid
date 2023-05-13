#pragma once

#include <chrono>
#include <thread>

#include "render.h"
#include "bna_group.h"

class RenderWindow  {

    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        ~RenderWindow();
        void addObject(RenderObject *pObject);
        void removeObject(RenderObject *pObject);
        void sortObjectsByPositionZ();

        void cleanUp();
        void clear();
        void modifyObjects(const AppState& state);
        void drawObjects();
        
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        std::vector<RenderObject *> m_vObjects;
};

template<class ValueType>
class ICallbacksRenderBNA {
    public:
        virtual bool onStart() = 0;
        virtual void doMutation() = 0;
        virtual void doTestAndRevert() = 0;
        virtual BNAGroup<ValueType>* getBNA() = 0;
        virtual const BNAStatCalcResults *getResults() = 0;
};

struct BNAItemPosition {
    int ylevel;
    int xlevel;
};

class RenderFpsCounter {
    public:
        RenderFpsCounter();
        void reset();
        bool addFrame();
        int getFps();

    private:
        long m_nNumberOfFrames;
        long m_nStartTime;
        long m_nElapsed;
        long m_nEvery;
        int m_nFps;
};

class RenderBNANode : public RenderRect {
    public:
        RenderBNANode(int nSizeNode);
        void setNodeInput(BNANodeInput *pNode);
        void setNode(BinaryNeuralAcidGraphNode *pNode, BNAGroup<BinaryNeuralAcidBit> *pBna);
        void setNodeOutput(BNANodeOutput *pNode, BNAGroup<BinaryNeuralAcidBit> *pBna);
        int getLevelY();

    private:
        BNANodeInput *m_pNodeInput;
        BinaryNeuralAcidGraphNode *m_pNode;
        BNANodeOutput *m_pNodeOutput;
        BNAItemPosition m_pos;
        int m_nSizeNode;
        int m_nLevelY;

        RenderColor *m_pColorNodeOutput;
        RenderColor *m_pColorOperAnd;
        RenderColor *m_pColorOperOr;
        RenderColor *m_pColorOperXor;
        RenderColor *m_pColorOperNxor;
};

template<class ValueType>
class RenderBNA {
    public:
        RenderBNA(ICallbacksRenderBNA<ValueType> *pCallbakcs) {
            TAG = "RenderBNA";
            m_pCallbacksRenderBNA = pCallbakcs;
            m_nWindowWidth = 1280;
            m_nWindowHeight = 720;
            m_nSizeNode = 6;
            m_nPadding = 100;
            m_vRenderPrevOutputResult = nullptr;
            m_vRenderCurrentOutputResult = nullptr;
        }

        bool run(const std::string &sWindowName) {
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

    private:
        void createFpsText() {
            m_pFpsText = new RenderAbsoluteTextBlock(CoordXY(50,20), "FPS: ----", 1000);
            m_pWindow->addObject(m_pFpsText);
            m_pWindow->sortObjectsByPositionZ();
        }

        void prepareVectorsSize() {
            BNAGroup<BinaryNeuralAcidBit> *pBna = m_pCallbacksRenderBNA->getBNA();
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

        void prepareRendorBNANodes() {
            BNAGroup<BinaryNeuralAcidBit> *pBna = m_pCallbacksRenderBNA->getBNA();
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
            const std::vector<BinaryNeuralAcidGraphNode *> &vNodes = pBna->getNodes();
            for (int i = 0; i < vNodes.size(); i++) {
                m_vRenderNodes[nInputSize + i]->setNode(vNodes[i], pBna);
            }
            const std::vector<BNANodeOutput *> &vNodesOutput = pBna->getNodesOutput();
            for (int i = 0; i < vNodesOutput.size(); i++) {
                m_vRenderNodes[nInputSize + nNodesSize + i]->setNodeOutput(vNodesOutput[i], pBna);
            }
        }

        int recurciveCalculateYLevel(int nInputCount, const std::vector<BinaryNeuralAcidGraphNode *> &vItems, BinaryNeuralAcidGraphNode *pItem, int nCounter) {
            if (pItem->getX() < nInputCount || pItem->getY() < nInputCount) {
                return nCounter + 1;
            }
            BinaryNeuralAcidGraphNode *pItemLeft = vItems[pItem->getX() - nInputCount];
            BinaryNeuralAcidGraphNode *pItemRight = vItems[pItem->getY() - nInputCount];
            int nLeft = recurciveCalculateYLevel(nInputCount, vItems, pItemLeft, nCounter + 1);
            int nRight = recurciveCalculateYLevel(nInputCount, vItems, pItemRight, nCounter + 1);
            return std::max(nLeft, nRight);
        }

        void updateInputNodesXY() {
            const std::vector<BinaryNeuralAcidGraphNode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getNodes();

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

        std::vector<RenderBNANode *> getChildAndParantNodes(int nIndex) {
            std::vector<RenderBNANode *> vRet;
            const std::vector<BinaryNeuralAcidGraphNode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
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

        std::vector<RenderBNANode *> getChildNodes(int nIndex) {
            std::vector<RenderBNANode *> vRet;
            const std::vector<BinaryNeuralAcidGraphNode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
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

        std::vector<RenderBNANode *> getParentNodes(int nIndex) {
            std::vector<RenderBNANode *> vRet;
            const std::vector<BinaryNeuralAcidGraphNode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
            int nInputCount = m_pCallbacksRenderBNA->getBNA()->getInputSize();
            int nCurrentIndex = nIndex - nInputCount;
            int nXIndex = vNodes[nCurrentIndex]->getX();
            int nYIndex = vNodes[nCurrentIndex]->getY();
            vRet.push_back(m_vRenderNodes[nXIndex]);
            vRet.push_back(m_vRenderNodes[nYIndex]);

            return vRet;
        }

        int distance(const CoordXY &p0, const CoordXY &p1) {
            int nRet = sqrt((p1.x()-p0.x())*(p1.x()-p0.x())+(p1.y()-p0.y())*(p1.y()-p0.y()));
            return nRet;
        }

        int distanceN(int n0, int n1) {
            int _n0 = std::min(n0,n1);
            int _n1 = std::max(n0,n1);
            return _n1 - _n0;
        }

        void updateMiddleNodesXY2() {
            const std::vector<BinaryNeuralAcidGraphNode *> &vItems = m_pCallbacksRenderBNA->getBNA()->getNodes();
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

        void updateOutputNodesXY() {
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

        void updateNodesConnections() {
            int nInputSize = m_pCallbacksRenderBNA->getBNA()->getInputSize();
            const std::vector<BinaryNeuralAcidGraphNode *> &vNodes = m_pCallbacksRenderBNA->getBNA()->getNodes();
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

        void prepareNodes() {
            this->prepareVectorsSize();
            this->updateInputNodesXY();
            this->updateMiddleNodesXY2();
            this->updateOutputNodesXY();
            this->updateNodesConnections();
        }

        void updateLine(int nIndexLine, RenderRect *pRect0, RenderRect *pRect1) {
            CoordXY p1 = pRect0->getCoord() + CoordXY(m_nSizeNode/2, m_nSizeNode/2);
            CoordXY p2 = pRect1->getCoord() + CoordXY(m_nSizeNode/2, m_nSizeNode/2);
            if (nIndexLine >= m_vRenderConnections.size()) {
                WsjcppLog::throw_err(TAG, "nIndexLine >= m_vRenderConnections.size()");
            }
            m_vRenderConnections[nIndexLine]->updateCoords(p1, p2);
            m_vRenderConnections[nIndexLine]->updateColor(pRect1->getColor());
            // return nIndexLine + 1;
        }

        std::string TAG;

        RenderWindow *m_pWindow;
        ICallbacksRenderBNA<ValueType> *m_pCallbacksRenderBNA;
        // std::vector<RenderRect *> m_vRenderNodes;
        std::vector<RenderBNANode *> m_vRenderNodes;
        std::vector<RenderConnection *> m_vRenderConnections;
        std::vector<RenderAbsoluteTextBlock *> m_vRenderCurrentOutputsResult;
        std::vector<RenderAbsoluteTextBlock *> m_vRenderPrevOutputsResult;
        RenderAbsoluteTextBlock * m_vRenderPrevOutputResult;
        RenderAbsoluteTextBlock * m_vRenderCurrentOutputResult;

        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nSizeNode;
        int m_nPadding;

        RenderAbsoluteTextBlock *m_pFpsText;
        RenderFpsCounter m_fps;
};