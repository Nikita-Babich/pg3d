

#ifndef _STRUCTS_INCLUDED_
#define _STRUCTS_INCLUDED_

// All the structs, their converters and basic math.

// Check if <vector.h> // Did not find VECTOR_H to test it
//#ifndef VECTOR_H
//#include <vector.h>
//#endif

#ifndef _MATH_H_
#include <math.h>
#endif

typedef struct {	// 2d
    int x;          // X-coordinate
    int y;          // Y-coordinate
    COLORREF color; // Color information
} Pixel;

typedef struct {	// used for vectors and points in 3d and 2d
    float x;          // X-coordinate
    float y;          // Y-coordinate
    float z;
    COLORREF color; // Color information
} Point;

typedef struct {
    Point start;          //
    Point finish;          //
    //COLORREF color; // Color information
} Segment;

typedef std::vector<Segment> Segments;
typedef std::vector<Point> Contour;
typedef std::vector<Contour> Objects;

//math
float scalarProduct(Point A, Point B){ return A.x*B.x + A.y*B.y + A.z*B.z; }
Point dif(Point A, Point B){ return (Point){B.x-A.x, B.y-A.y, B.z-A.z}; }
float len(Point A){ return sqrt(scalarProduct(A,A));}
Point normalise(Point A){ float L = len(A); return (Point){A.x/L, A.y/L, A.z/L};}

//from 2d
Point orthoVector(Point A, Point B){Point d = dif(A,B); return (Point){-d.y,d.x};} //only 2d
Segment orthoSegment(Segment A){ //only 2d, originally for testing
	Point newd = orthoVector(A.start, A.finish);
	A.finish.x = A.start.x + newd.x;
	A.finish.y = A.start.y + newd.y;
	return A;
}


// Converters
Pixel convertPointToPixel(Point p) { return (Pixel){(int)p.x, (int)p.y}; }
Point convertPixelToPoint(Pixel px) { return (Point){(float)px.x, (float)px.y}; }

//random creation
Pixel rpi(){  return (Pixel){rand() % DRAW_WIDTH, rand() % DRAW_HEIGHT}; } //random Pixel 
Point rpo(){ return (Point){(float)(rand() % DRAW_WIDTH), (float)(rand() % DRAW_HEIGHT)}; } //random Point
Point rpo3(){ return (Point){(float)(rand() % 10), (float)(rand() % 10), (float)(rand() % 10)}; } //random Point

Segment rs(){ return (Segment){ convertPixelToPoint(rpi()), convertPixelToPoint(rpi()) }; }; // random segment
Segments rss(int size = 10){
	Segments result;
	for(size_t i=0; i<size; i++){
		Segment segment = rs();
		result.push_back(segment);
	}
	return result;
}
Contour rcont(int size){
	Contour result;
	for(size_t i=0; i<size; i++){
		Point p = rpo();
		result.push_back(p);
	}
	return result;
}

#endif // STRUCTS_INCLUDED