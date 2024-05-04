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

//triangle section
COLORREF colorchooser(float x, float y, Contour C){
	Point P = (Point){x,y};
	if(triangle_method){ //blend
		float L0 = abs(
			(C[1].x-P.x) * (C[2].y-P.y) - (C[1].y - P.y) * (C[2].x - P.x)
			) / abs(
			(C[1].x-C[0].x) * (C[2].y-C[0].y) - (C[1].y - C[0].y) * (C[2].x - C[0].x)
		);
		float L1 = abs(
			(C[0].x-P.x) * (C[2].y-P.y) - (C[0].y - P.y) * (C[2].x - P.x)
			) / abs(
			(C[1].x-C[0].x) * (C[2].y-C[0].y) - (C[1].y - C[0].y) * (C[2].x - C[0].x)
		);
		float L2 = 1 - L0 - L1;
		int R1, G1, B1, R2, G2, B2, R3, G3, B3;
    	ExtractRGBComponents(RED, &R1, &G1, &B1);
    	ExtractRGBComponents(GREEN, &R2, &G2, &B2);
    	ExtractRGBComponents(BLUE, &R3, &G3, &B3);
		
		int R = (int)(L0 * R1 + L1 * R2 + L2 * R3);
    	int G = (int)(L0 * G1 + L1 * G2 + L2 * G3);
    	int B = (int)(L0 * B1 + L1 * B2 + L2 * B3);
    	R = (R < 0) ? 0 : ((R > 255) ? 255 : R);
    	G = (G < 0) ? 0 : ((G > 255) ? 255 : G);
    	B = (B < 0) ? 0 : ((B > 255) ? 255 : B);
    	
    	return RGB(R, G, B);
    
	} else { //closest point
		float d0 = dist(P,C[0]);
		float d1 = dist(P,C[1]);
		float d2 = dist(P,C[2]);
		if (d0<d1 && d0 < d2) return RED;
		if (d1<d0 && d1 < d2) return GREEN;
		if (d2<d0 && d2 < d1) return BLUE;
	};
	return RED;
}
void fill_sliced_triangle(Contour C, Contour orig){
	Segment e1, e2; float we1, we2;
	float y = e1.start.y; float ymax = e1.finish.y;
	float x1 = e1.start.x; float x2 = e2.start.x;
	if(C[0].y == C[1].y){
		e1 = (Segment){ C[0], C[2] };
		e2 = (Segment){ C[1], C[2] };
		we1 = (C[2].x - C[0].x) / (C[2].y - C[0].y);
		we2 = (C[2].x - C[1].x) / (C[2].y - C[1].y);
		y = e1.start.y;
		ymax = e1.finish.y;
		x1 = e1.start.x;
		x2 = e2.start.x;
	} else {
		e1 = (Segment){ C[0], C[1] };
		e2 = (Segment){ C[0], C[2] };
		we1 = (C[1].x - C[0].x) / (C[1].y - C[0].y);
		we2 = (C[2].x - C[0].x) / (C[2].y - C[0].y);
		y = e2.start.y;
		ymax = e2.finish.y;
		x1 = e1.start.x;
		x2 = e2.start.x;
	};
	while(y < ymax){
		for(float i = std::min(x1, x2); i <= std::max(x1, x2); i=i+1){
			DrawPixel(static_cast<int>(i),static_cast<int>(y), colorchooser(i,y,orig));
		}
		x1 += we1;
		x2 += we2;
		y+=1;
	}
}
void fill_triangle(Contour C, Contour orig){
	std::sort(C.begin(), C.end(), [](const Point& a, const Point& b) {
        if (a.y == b.y) return a.x < b.x;
        return a.y < b.y;
    });
    if(C[0].y == C[1].y || C[1].y == C[2].y) {
    	fill_sliced_triangle(C, orig);
	} else { // slicing
		float m = (C[2].y - C[0].y)/(C[2].x - C[0].x);
		Point P = (Point){
			(C[1].y-C[0].y)/m + C[0].x,
			C[1].y
		};
		Contour upper;
		Contour lower;
		if(C[1].x < P.x){
			upper.push_back(C[0]); upper.push_back(C[1]); upper.push_back(P);
			lower.push_back(C[1]); lower.push_back(P); lower.push_back(C[2]);
			fill_sliced_triangle(upper, orig);
			fill_sliced_triangle(lower, orig);
		} else {
			upper.push_back(C[0]); upper.push_back(P); upper.push_back(C[1]);
			lower.push_back(P); lower.push_back(C[1]); lower.push_back(C[2]);
			fill_sliced_triangle(upper, orig);
			fill_sliced_triangle(lower, orig);
		}
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

