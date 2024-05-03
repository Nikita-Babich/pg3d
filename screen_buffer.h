//buffer to draw to the screen full picture at once

#ifndef _BUFFER_INCLUDED_
#define _BUFFER_INCLUDED_

// Check if <windows.h> and colors.h is already included
#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef _COLORS_INCLUDED_
#include "colors.h" 
#endif


#define UPDATE {\
			PAINTSTRUCT ps;\
    		HDC hdc;\
			hdc = BeginPaint(hwnd, &ps); \
        	UpdateScreen(hdc);\
        	EndPaint(hwnd, &ps);};
        	

BITMAPINFO bmi;
void InitializeBitmapInfo(BITMAPINFO* bmi) {
    ZeroMemory(bmi, sizeof(BITMAPINFO));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = WINDOW_WIDTH;
    bmi->bmiHeader.biHeight = -WINDOW_HEIGHT;  // Use negative height for top-down bitmap
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 32;  // Assuming you are using 32-bit color depth (COLORREF)
    bmi->bmiHeader.biCompression = BI_RGB;
};

COLORREF buffer[WINDOW_HEIGHT][WINDOW_WIDTH];
// Function to initialize buffer
void InitializeBuffer() {
    //buffer = (COLORREF*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(COLORREF));
    if (buffer == NULL) {
        // memory allocation failure, no idea ho to handle
    }
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        // Iterate over each column in the current row
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            // Set the color of the current pixel in the buffer
            buffer[i][j] = background_color; //  background_color or RGB(255, 0, 0)
        }
    }
    // initialize buffer to background color or any default color
    //memset(buffer, RGB(255, 100, 255), WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(COLORREF));
    //printf( "\n buffer has %d %d %d", buffer[0], buffer[1], buffer[2]);
}
// Function to draw a pixel in buffer
void DrawPixel(int x, int y, COLORREF color) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
        buffer[y][x]= color;
    }
}
void DrawPoint(Point A, COLORREF color){
	DrawPixel(static_cast<int>(A.x),static_cast<int>(A.y),color); //was float
}
void dda2( Pixel start, Pixel end, COLORREF color) {
	int x1 = start.x; int y1 = start.y;
	int x2 = end.x; int y2 = end.y;
	int DX = x2 - x1;
	int DY = y2 - y1;
	float dx, dy;
	int steps; //equals longest dimension
	if (abs(DX) >= abs(DY)) steps = abs(DX);
		else steps = abs(DY);
	dx = DX / (float)steps;
	dy = DY / (float)steps;
	float x = x1;
	float y = y1;
	int i = 0;
	while (i <= steps) {
		//SetPixel(hdc, static_cast<int>(x), static_cast<int>(y), color);
		DrawPixel(static_cast<int>(x), static_cast<int>(y), color);
		x = x + dx;
		y = y + dy;
		i++;
	}
};
// Function to transfer buffer to screen
void UpdateScreen(HDC hdc) {
    if (buffer != NULL) {
    	//printf("\n buffer exists, update called ");
        SetDIBitsToDevice(hdc,  0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, WINDOW_HEIGHT, buffer, &bmi, DIB_RGB_COLORS); //DIB_PAL_COLORS
    }
}

#endif // BUFFER_INCLUDED

