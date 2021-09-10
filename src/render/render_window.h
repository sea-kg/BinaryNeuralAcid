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
        virtual std::vector<int> &getPrevCounters() = 0;
        virtual int getDataTestsSize() = 0;
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
        int recurciveCalculateYLevel(int nInputCount, const std::vector<BNAItem *> &vItems, BNAItem *pItem, int nCounter);
        void prepareLevels();
        void updateInputNodesXY();
        void updateMiddleNodesXY();
        std::vector<RenderRect *> getChildAndParantNodes(int nIndex);
        void updateMiddleNodesXY2();
        void updateOutputNodesXY();
        void updateNodesConnections();
        void prepareNodes();
        int updateLine(int nIndexLine, RenderRect *, RenderRect *);
        void updateColorNode(int nIndexNode, const std::string &sOperType);

        RenderWindow *m_pWindow;
        ICallbacksRenderBNA *m_pCallbacksRenderBNA;
        std::vector<RenderRect *> m_vRenderNodes;
        std::vector<RenderConnection *> m_vRenderConnections;
        std::vector<BNAItemPosition> m_vNodesPositions;
        std::vector<RenderAbsoluteTextBlock *> m_vRenderOutputsResult;
        RenderAbsoluteTextBlock * m_vRenderOutputResult;

        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nSizeNode;
        int m_maxYLevel;
        int m_nPadding;
        std::vector<int> m_maxXLevels;

        RenderColor *m_pColorOperAnd;
        RenderColor *m_pColorOperOr;
        RenderColor *m_pColorOperXor;
        RenderColor *m_pColorOperNxor;
};