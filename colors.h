

#ifndef _COLORS_INCLUDED_
#define _COLORS_INCLUDED_

//Colors

COLORREF BLACK = RGB(0, 0, 0); 
COLORREF WHITE = RGB(255, 255, 255); 
COLORREF RED = RGB(0, 0, 255);
COLORREF BLUE = RGB(255, 0, 0);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF PINK = RGB(255, 0, 255);
COLORREF main_color = BLACK;
COLORREF background_color = WHITE;

COLORREF diffusion;
COLORREF mirror;
COLORREF ambience;

COLORREF rc(){ return RGB(rand()%255, rand()%255, rand()%255); };

void unpackColorRef(COLORREF color, int& red, int& green, int& blue) {
    // Extract the blue component (least significant byte)
    blue = color & 0xFF;
    // Extract the green component (middle byte)
    green = (color >> 8) & 0xFF;
    // Extract the red component (most significant byte)
    red = (color >> 16) & 0xFF;
}

#endif // COLORS_INCLUDED