#pragma once
#include "render.h"
#include "bna.h"

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

class ICallbacksRenderBNA {
    public:
        virtual bool onStart() = 0;
        virtual void doMutation() = 0;
        virtual void doTestAndRevert() = 0;
        virtual BNA* getBNA() = 0;
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
        void setNode(BNANode *pNode, BNA *pBna);
        void setNodeOutput(BNANodeOutput *pNode, BNA *pBna);
        int getLevelY();

    private:
        BNANodeInput *m_pNodeInput;
        BNANode *m_pNode;
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

class RenderBNA {
    public:
        RenderBNA(ICallbacksRenderBNA *);
        bool run(const std::string &sWindowName);

    private:
        void createFpsText();
        void prepareVectorsSize();
        void prepareRendorBNANodes();
        int recurciveCalculateYLevel(int nInputCount, const std::vector<BNANode *> &vItems, BNANode *pItem, int nCounter);
        void updateInputNodesXY();
        std::vector<RenderBNANode *> getChildAndParantNodes(int nIndex);
        std::vector<RenderBNANode *> getChildNodes(int nIndex);
        std::vector<RenderBNANode *> getParentNodes(int nIndex);
        int distance(const CoordXY &p0, const CoordXY &p1);
        int distanceN(int n0, int n1);
        void updateMiddleNodesXY2();
        void updateOutputNodesXY();
        void updateNodesConnections();
        void prepareNodes();
        void updateLine(int nIndexLine, RenderRect *, RenderRect *);

        std::string TAG;

        RenderWindow *m_pWindow;
        ICallbacksRenderBNA *m_pCallbacksRenderBNA;
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