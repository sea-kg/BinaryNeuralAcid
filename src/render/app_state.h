#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "json.hpp"

class CoordXY {
    public:
        CoordXY();
        CoordXY(int x, int y);
        int x() const;
        int y() const;
        void update(int x, int y);

        CoordXY& operator+=(const CoordXY& other);
        CoordXY& operator-=(const CoordXY& other);
        CoordXY operator+(const CoordXY& other) const;
        CoordXY operator-(const CoordXY& other) const;
    private:
        int m_nX, m_nY;
};

class AppState {

    public:
        AppState(int windowWidth, int windowHeight);
        void init();
        void updateElapsedTime();
        long getElapsedTime() const;
        const CoordXY &getCoordLeftTop() const;
        void incrementCoordLeftTopX(int nX);
        void incrementCoordLeftTopY(int nY);
        
        const int windowWidth() const;
        const int windowHeight() const;

        void setMouseCaptured(bool bMouseCaptured);
        bool isMouseCaptured() const;

    private:
        bool m_bMouseCaptured;
        long m_nElapsedTime;
        long m_nStartTime;
        CoordXY m_coordLeftTop;
        int m_nWindowWidth;
        int m_nWindowHeight;
};