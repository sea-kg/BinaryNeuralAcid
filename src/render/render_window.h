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
        virtual void doIterattion() = 0;
        virtual BNA* getBNA() = 0;
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
        void prepareYLevels();
        void prepareXLevels();
        void prepareNodes();
        int updateLine(int nIndexLine, RenderRect *, RenderRect *);

        RenderWindow *m_pWindow;
        ICallbacksRenderBNA *m_pCallbacksRenderBNA;
        std::vector<RenderRect *> m_vRenderNodes;
        std::vector<RenderLine *> m_vRenderConnections;
        std::vector<BNAItemPosition> m_vNodesPositions;
        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nSizeNode;
        int m_maxYLevel;
        std::vector<int> m_maxXLevels;

        RenderColor *m_pColorOperAnd;
        RenderColor *m_pColorOperOr;
        RenderColor *m_pColorOperXor;
        RenderColor *m_pColorOperNxor;
};