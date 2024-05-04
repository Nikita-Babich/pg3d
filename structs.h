

#ifndef _STRUCTS_INCLUDED_
#define _STRUCTS_INCLUDED_

// All the structs, their converters, basic math and operator overloads

// Check if <vector.h> // Did not find VECTOR_H to test it
//#ifndef VECTOR_H
//#include <vector.h>
//#endif

#ifndef _MATH_H_
#include <math.h>
#endif

enum Direction {
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	FORWARD = 4,
	BACKWARD = 5
};


typedef struct {	// 2d
    int x;          // X-coordinate
    int y;          // Y-coordinate
    COLORREF color; // Color information
} Pixel;

struct Face;
struct Point;

typedef struct {	// used for vectors and points in 3d and 2d
    float x;          // X-coordinate
    float y;          // Y-coordinate
    float z;
    COLORREF color; // Color information
    Point normal;
    std::vector<Face*> facePtrs;
} Point;

typedef struct {	// used for triangles in 3d
    Point* A;          // X-coordinate
    Point* B;         // Y-coordinate
    Point* C;
     
    Point normal;
} Face;

typedef struct {
    Point start;          //
    Point finish;          //
    //COLORREF color; // Color information
} Segment;

typedef std::vector<Segment> Segments;
typedef std::vector<Point> Contour;
typedef std::vector<Point> Allpoints;
typedef std::vector<Contour> Objects;
typedef std::vector<Face> Scene;

//global object;
Scene scene;
Allpoints allpoints;
Point light;
bool Pmode = false; //plane vs spherical projection
bool Dmode = false; //wireframe vs coloring

//math
Point operator+(const Point& p1, const Point& p2) {
    Point result;
    result.x = p1.x + p2.x;
    result.y = p1.y + p2.y;
    result.z = p1.z + p2.z;
    //result.color = p1.color;
    return result;
}
Point operator-(const Point& p1, const Point& p2) {
    Point result;
    result.x = p1.x - p2.x;
    result.y = p1.y - p2.y;
    result.z = p1.z - p2.z;
    //result.color = p1.color;
    return result;
}
Point operator-(const Point& p1) {
    Point result;
    result.x = - p1.x ;
    result.y = - p1.y ;
    result.z = - p1.z ;
    return result;
}
Point operator*(float num, const Point& p2) {
    Point result;
    result.x = num * p2.x;
    result.y = num * p2.y;
    result.z = num * p2.z;
    result.color = p2.color;
    return result;
}
float operator*(const Point& A, const Point& B) { //scalar multiplication
    float result = A.x*B.x + A.y*B.y + A.z*B.z
    return result;
}
float operator^(const Point& A, const Point& B) { //angle between vectors
	return asin( (A*B) / (len(A) * len(B)) );
}
Point& operator+=(Point& lhs, const Point& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
}
Point crossProduct(const Point& u, const Point& v) {
    Point cross;
    cross.x = u.y * v.z - u.z * v.y;
    cross.y = u.z * v.x - u.x * v.z;
    cross.z = u.x * v.y - u.y * v.x;
    return cross;
}



float scalarProduct(Point A, Point B){ return A.x*B.x + A.y*B.y + A.z*B.z; }
Point dif(Point A, Point B){ return (Point){B.x-A.x, B.y-A.y, B.z-A.z}; }
float len(Point A){ return sqrt(scalarProduct(A,A));}
Point normalise(Point A){ 
	float L = len(A); 
	if(L>0) {
		return (Point){A.x/L, A.y/L, A.z/L};
	} else {
		return (Point){0, 0, 0};
	}
}
void calculateFaceNormal(Face& face) {
    //Point u = {face.B->x - face.A->x, face.B->y - face.A->y, face.B->z - face.A->z};
    //Point v = {face.C->x - face.A->x, face.C->y - face.A->y, face.C->z - face.A->z};
    Point u = *(face.B) - *(face.A);
    Point v = *(face.C) - *(face.A);
    face.normal = crossProduct(u, v);
    face.normal = normalise(face.normal);
    if ((*(face.A)) * face.normal < 0){
    	face.normal = -1 * face.normal;
	}
}
void calculatePointNormal(Point& point) {
    // Initialize the normal vector
    point.normal = {0, 0, 0};

    // Iterate over all faces the point is part of
    for (Face* face : point.facePtrs) {
        // Accumulate the normals of the connected faces
        point.normal.x += face.normal.x;
        point.normal.y += face.normal.y;
        point.normal.z += face.normal.z;
    }

    // Normalize the accumulated normal
    point.normal = normalise(point.normal);
}
void calculateNormals(Allpoints& allpoints, Scene& scene) {
    // Calculate normals for each face
    for (Face& face : scene) {
        calculateFaceNormal(face);
    }

    // Calculate normals for each point
    for (Point& point : allpoints) {
        calculatePointNormal(point);
    }
}

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
Contour FaceToContour(Face face){
	Contour result;
	result.push_back(face.A);
	result.push_back(face.B);
	result.push_back(face.C);
	return result;
}




#endif // STRUCTS_INCLUDED