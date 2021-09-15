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

class RenderBNA {
    public:
        RenderBNA(ICallbacksRenderBNA *);
        bool run();

    private:
        void prepareVectorsSize();
        int recurciveCalculateYLevel(int nInputCount, const std::vector<BNANode *> &vItems, BNANode *pItem, int nCounter);
        void updateInputNodesXY();
        std::vector<RenderRect *> getChildAndParantNodes(int nIndex);
        int distance(const CoordXY &p0, const CoordXY &p1);
        int distanceN(int n0, int n1);
        void updateMiddleNodesXY2();
        void updateOutputNodesXY();
        void updateNodesConnections();
        void prepareNodes();
        int updateLine(int nIndexLine, RenderRect *, RenderRect *);
        void updateColorNode(int nIndexNode, const std::string &sOperType);

        std::string TAG;

        RenderWindow *m_pWindow;
        ICallbacksRenderBNA *m_pCallbacksRenderBNA;
        std::vector<RenderRect *> m_vRenderNodes;
        std::vector<RenderConnection *> m_vRenderConnections;
        std::vector<BNAItemPosition> m_vNodesPositions;
        std::vector<RenderAbsoluteTextBlock *> m_vRenderCurrentOutputsResult;
        std::vector<RenderAbsoluteTextBlock *> m_vRenderPrevOutputsResult;
        RenderAbsoluteTextBlock * m_vRenderPrevOutputResult;
        RenderAbsoluteTextBlock * m_vRenderCurrentOutputResult;

        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nSizeNode;
        int m_nPadding;

        RenderColor *m_pColorNodeOutput;
        RenderColor *m_pColorOperAnd;
        RenderColor *m_pColorOperOr;
        RenderColor *m_pColorOperXor;
        RenderColor *m_pColorOperNxor;
};