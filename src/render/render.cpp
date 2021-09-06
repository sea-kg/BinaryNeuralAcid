#include "render.h"
#include "wsjcpp_core.h"
#include <cmath>
#define PI 3.14159265

// ---------------------------------------------------------------------
// RenderColor

RenderColor::RenderColor(int nR, int nG, int nB, int nA) {
    m_nR = nR;
    m_nG = nG;
    m_nB = nB;
    m_nA = nA;
}

void RenderColor::changeRenderColor(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_nR, m_nG, m_nB, m_nA);
}

// ---------------------------------------------------------------------
// RenderObject

RenderObject::RenderObject(int nPositionZ) {
    m_nPositionZ = nPositionZ;
}

int RenderObject::getPositionZ() {
    return m_nPositionZ;
}

void RenderObject::modify(const AppState& state) {
    // nothing modify in base object
}

// ---------------------------------------------------------------------
// RenderLine

RenderLine::RenderLine(const CoordXY &p1, const CoordXY &p2, const RenderColor &color, int nPositionZ) 
: m_color(color), RenderObject(nPositionZ) {
    m_coord1 = p1;
    m_coord2 = p2;
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

void RenderLine::modify(const AppState& state) {
    m_coord1 = state.getCoordLeftTop() + m_startCoord1;
    m_coord2 = state.getCoordLeftTop() + m_startCoord2;
}

void RenderLine::draw(SDL_Renderer* renderer) {
    m_color.changeRenderColor(renderer);
    SDL_RenderDrawLine(renderer, m_coord1.x(), m_coord1.y(), m_coord2.x(), m_coord2.y());
}

const CoordXY &RenderLine::getAbsoluteCoord1() {
    return m_startCoord1;
}

const CoordXY &RenderLine::getAbsoluteCoord2() {
    return m_startCoord2;
}

const CoordXY &RenderLine::getCoord1() {
    return m_coord1;
}

const CoordXY &RenderLine::getCoord2() {
    return m_coord2;
}

void RenderLine::updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2) {
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

void RenderLine::updateColor(const RenderColor &color) {
    m_color = color;
}

// ---------------------------------------------------------------------
// RenderRect

RenderRect::RenderRect(const CoordXY &p1, int w, int h, const RenderColor &color, int nPositionZ)
: RenderObject(nPositionZ), m_color(color) {
    m_coord1 = p1;
    m_nW = w;
    m_nH = h;
}

void RenderRect::modify(const AppState& state) {
    // nothing
}

void RenderRect::draw(SDL_Renderer* renderer) {
    SDL_Rect srcrect;
    srcrect.x = m_coord1.x();
    srcrect.y = m_coord1.y();
    srcrect.w = m_nW;
    srcrect.h = m_nH;
    m_color.changeRenderColor(renderer);
    SDL_RenderFillRect(renderer, &srcrect);
}

bool RenderRect::hasPoint(const CoordXY &p0) {
    return
        p0.x() >= m_coord1.x() && p0.x() <= m_coord1.x() + m_nW
        && p0.y() >= m_coord1.y() && p0.y() <= m_coord1.y() + m_nH;
}

void RenderRect::updateCoord(const CoordXY &p0, int w, int h) {
    m_coord1 = p0;
    m_nW = w;
    m_nW = h;
}

void RenderRect::updateXY(int x, int y) {
    m_coord1.update(x, y);
}

const CoordXY &RenderRect::getCoord() {
    return m_coord1;
}

void RenderRect::updateColor(const RenderColor &color) {
    m_color = color;
}

const RenderColor &RenderRect::getColor() {
    return m_color;
}

// ---------------------------------------------------------------------
// RenderMouse

RenderMouse::RenderMouse(
    const CoordXY &p1,
    const RenderColor &color,
    int nPositionZ
) : RenderObject(nPositionZ),
    m_p1(p1),
    m_color(color),
    m_nCursorType(0)
{
    m_pDiff2 = CoordXY(10,10);
    m_pDiff3 = CoordXY(7,3);
    m_pDiff4 = CoordXY(3,7);

    m_pLine1 = new RenderLine(p1, p1, color);
    m_pLine2 = new RenderLine(p1, p1, color);
    m_pLine3 = new RenderLine(p1, p1, color);

    m_pLineMoveble1 = new RenderLine(p1, p1, color);
    m_pLineMoveble2 = new RenderLine(p1, p1, color);

    this->updateCoord(p1);
}

void RenderMouse::modify(const AppState& state) {
    m_pLine1->modify(state);
    m_pLine2->modify(state);
    m_pLine3->modify(state);
    m_pLineMoveble1->modify(state);
    m_pLineMoveble2->modify(state);
}

void RenderMouse::draw(SDL_Renderer* renderer) {
    if (m_nCursorType == 0) {
        m_pLine1->draw(renderer);
        m_pLine2->draw(renderer);
        m_pLine3->draw(renderer);
    } else if (m_nCursorType == 1) {
        m_pLineMoveble1->draw(renderer);
        m_pLineMoveble2->draw(renderer);
    }
}

void RenderMouse::updateCoord(const CoordXY &p0) {
    // arrow
    CoordXY p1 = p0;
    CoordXY p2 = p1 + m_pDiff2;
    CoordXY p3 = p1 + m_pDiff3;
    CoordXY p4 = p1 + m_pDiff4;
    
    m_pLine1->updateAbsoluteCoords(p1,p2);
    m_pLine2->updateAbsoluteCoords(p1,p3);
    m_pLine3->updateAbsoluteCoords(p1,p4);

    // moveble
    CoordXY p5 = p1 - CoordXY(0,5);
    CoordXY p6 = p1 + CoordXY(0,5);
    CoordXY p7 = p1 - CoordXY(5,0);
    CoordXY p8 = p1 + CoordXY(5,0);
    m_pLineMoveble1->updateAbsoluteCoords(p5,p6);
    m_pLineMoveble2->updateAbsoluteCoords(p7,p8);
}

void RenderMouse::changeCursorToArrow() {
    m_nCursorType = 0;
}

void RenderMouse::changeCursorToMoveble() {
    m_nCursorType = 1;
}

// ---------------------------------------------------------------------
// RenderTextBlock

RenderAbsoluteTextBlock::RenderAbsoluteTextBlock(const CoordXY &p0, const std::string &sText, int nPositionZ)
: RenderObject(nPositionZ) {
    m_coordCenter = p0;
    m_sText = sText;
    m_sUpdateText = m_sText;
    m_pFont = TTF_OpenFont("./res/fonts/roboto/Roboto-Black.ttf", 18);
    m_color = {255, 255, 255};
    if (!m_pFont) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
}

void RenderAbsoluteTextBlock::modify(const AppState& state) {
    // nothing i think
    if (m_sUpdateText != m_sText) {
        m_sText = m_sUpdateText;
    }
};

void RenderAbsoluteTextBlock::draw(SDL_Renderer* renderer) {
    // int w,h;
    // if (TTF_SizeText(font,"Hello World!",&w,&h)) {
        // perhaps print the current TTF_GetError(), the string can't be rendered...
    // } else {
    //    printf("width=%d height=%d\n",w,h);
    // }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(m_pFont, m_sText.c_str(), m_color);
    // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = m_coordCenter.x();  //controls the rect's x coordinate 
    Message_rect.y = m_coordCenter.y(); // controls the rect's y coordinte
    Message_rect.w = surfaceMessage->w; // controls the width of the rect
    Message_rect.h = surfaceMessage->h; // controls the height of the rect

    // Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

    // Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
};

void RenderAbsoluteTextBlock::updateText(const std::string &sNewText) {
    m_sUpdateText = sNewText;
}
