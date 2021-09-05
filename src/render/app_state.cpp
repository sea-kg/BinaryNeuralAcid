#include "app_state.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// CoordXY

CoordXY::CoordXY() {
    m_nX = 0;
    m_nY = 0;
}

CoordXY::CoordXY(int x, int y) {
    m_nX = x;
    m_nY = y;
}

int CoordXY::x() const {
    return m_nX;
}

int CoordXY::y() const {
    return m_nY;
}

void CoordXY::update(int x, int y) {
    m_nX = x;
    m_nY = y;
}

CoordXY& CoordXY::operator+=(const CoordXY& other) {
    m_nX = m_nX + other.x();
    m_nY = m_nY + other.y();
    return *this;
}

CoordXY& CoordXY::operator-=(const CoordXY& other) {
    m_nX = m_nX - other.x();
    m_nY = m_nY - other.y();
    return *this;
}

CoordXY CoordXY::operator+(const CoordXY& other) const {
    CoordXY c(m_nX, m_nY);
    c += other;
    return c;
}

CoordXY CoordXY::operator-(const CoordXY& other) const {
    CoordXY c(m_nX, m_nY);
    c -= other;
    return c;
}

// ---------------------------------------------------------------------
// AppState

AppState::AppState(int windowWidth, int windowHeight) {
    m_nStartTime = 0;
    m_nElapsedTime = 0;
    m_nWindowWidth = windowWidth;
    m_nWindowHeight = windowHeight;
    m_bMouseCaptured = false;
}

void AppState::init() {
    m_nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
}

void AppState::updateElapsedTime() {
    m_nElapsedTime = WsjcppCore::getCurrentTimeInMilliseconds() - m_nStartTime;
}

long AppState::getElapsedTime() const {
    return m_nElapsedTime;
}

const CoordXY &AppState::getCoordLeftTop() const {
    return m_coordLeftTop;
}

void AppState::incrementCoordLeftTopX(int nX) {
    m_coordLeftTop += CoordXY(nX, 0);
}

void AppState::incrementCoordLeftTopY(int nY) {
    m_coordLeftTop += CoordXY(0, nY);
}

const int AppState::windowWidth() const {
    return m_nWindowWidth;
}

const int AppState::windowHeight() const {
    return m_nWindowHeight;
}

void AppState::setMouseCaptured(bool bMouseCaptured) {
    m_bMouseCaptured = bMouseCaptured;
    SDL_SetRelativeMouseMode(m_bMouseCaptured ? SDL_TRUE : SDL_FALSE);
}

bool AppState::isMouseCaptured() const {
    return m_bMouseCaptured;
}