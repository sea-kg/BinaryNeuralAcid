#pragma once
#include "app_state.h"
#include <vector>
#include <string>

class RenderColor {
    public:
        RenderColor(int nR, int nG, int nB, int nA);
        void changeRenderColor(SDL_Renderer* renderer);

    private:
        int m_nR;
        int m_nG;
        int m_nB;
        int m_nA;
};

class RenderObject {

    public:
        RenderObject(int nPositionZ);
        int getPositionZ();
        virtual void modify(const AppState& state);
        virtual void draw(SDL_Renderer* pRenderer) = 0;

    private:
        int m_nPositionZ;
};

class RenderLine : public RenderObject {

    public:
        RenderLine(const CoordXY &p1, const CoordXY &p2, const RenderColor &color, int nPositionZ = 0);
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        
        const CoordXY &getAbsoluteCoord1();
        const CoordXY &getAbsoluteCoord2();
        const CoordXY &getCoord1();
        const CoordXY &getCoord2();
        void updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2);

    private:
        RenderColor m_color;
        CoordXY m_startCoord1;
        CoordXY m_startCoord2;
        CoordXY m_coord1;
        CoordXY m_coord2;
};

class RenderRect : public RenderObject {

    public:
        RenderRect(const CoordXY &p1, int w, int h, const RenderColor &color = RenderColor(255,255,255,255), int nPositionZ = 0);
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        bool hasPoint(const CoordXY &p0);
        void updateCoord(const CoordXY &p0, int w, int h);
    private:
        CoordXY m_coord1;
        int m_nW;
        int m_nH;

        RenderColor m_color;
};

class RenderTriangle : public RenderObject {

    public:
        RenderTriangle(
            const CoordXY &p1,
            const CoordXY &p2,
            const CoordXY &p3,
            const RenderColor &color = RenderColor(255,255,255,255),
            int nPositionZ = 0
        );
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        RenderLine m_line1;
        RenderLine m_line2;
        RenderLine m_line3;
        RenderColor m_color;
        CoordXY m_middlePoint;
};

class RenderArea : public RenderObject {

    public:
        RenderArea(
            const std::vector<CoordXY> &vPoints,
            const RenderColor &color = RenderColor(255,255,255,255),
            int nPositionZ = 0
        );
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

        bool hasMoveblePoint(const CoordXY &p0, RenderRect *&pRect);
        void updatePointCoord(RenderRect *pRect, const CoordXY &newCoord);
        const std::vector<CoordXY> &getPoints();

    private:
        std::vector<CoordXY> m_vPoints;
        std::vector<RenderLine *> m_vLines;
        std::vector<RenderRect *> m_vRects;
        RenderColor m_color;
        int m_nRectBorderSize;
};

class RenderMouse : public RenderObject {

    public:
        RenderMouse(
            const CoordXY &p1,
            const RenderColor &color = RenderColor(255,255,255,255),
            int nPositionZ = 0
        );
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateCoord(const CoordXY &p0);
        void changeCursorToArrow();
        void changeCursorToMoveble();
        

    private:
        int m_nCursorType;
        CoordXY m_p1;
        CoordXY m_pDiff2;
        CoordXY m_pDiff3;
        CoordXY m_pDiff4;
        RenderLine *m_pLine1;
        RenderLine *m_pLine2;
        RenderLine *m_pLine3;

        RenderLine *m_pLineMoveble1;
        RenderLine *m_pLineMoveble2;

        RenderColor m_color;
        CoordXY m_middlePoint;
};

class RenderAbsoluteTextBlock : public RenderObject {

    public:
        RenderAbsoluteTextBlock(
            const CoordXY &p0,
            const std::string &sText,
            int nPositionZ = 0
        );
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateText(const std::string &sNewText);

    private:
        CoordXY m_coordCenter;
        std::string m_sText;
        std::string m_sUpdateText;
        TTF_Font* m_pFont;
        SDL_Color m_color;

        SDL_Rect currentFrame;
};
