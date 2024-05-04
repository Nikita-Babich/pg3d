

#ifndef _CAMERA_INCLUDED_
#define _CAMERA_INCLUDED_

#ifndef _STRUCTS_INCLUDED_
#include "structs.h"
#endif


//there is only one camera at any instance, so instead of functions procedures are used here
// Camera should change in place to be the global object
float FOV = 90;
typedef struct {	// 
    Point pos;          //position
    float beta; 		//horisontal angle
    float alpha;		//vertical angle, there is no roll
    
    // result of orientation calculation, 3 relative vectors
    Point forw;   //from y
    Point right; //from x
    Point up; //from z
    
    //float w=FOV;//angles of the FOV
    //float h=FOV;
} Camera;

Camera camera;
camera.pos = {0,0,0};
camera.beta = 0; // grows towards left
camera.alpha = 0; // grows towards up

float fov = M_PI/2;
float scaling = 100;

Point x_const = {1,0,0};
Point y_const = {0,1,0};
Point up_const = {0,0,1};

Point rotateAroundAxis(Point p, Point axis, float angle) {
    // Normalize the axis vector
    axis = normalise(axis);
    float ux, uy, uz;
    ux = axis.x;
    uy = axis.y;
    uz = axis.z;

    // Calculate cosine and sine of the angle
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    // Calculate one minus cosine
    float oneMinusCosTheta = 1.0f - cosTheta;

    // Calculate the rotation matrix components
    float r00 = cosTheta + ux * ux * oneMinusCosTheta;
    float r01 = ux * uy * oneMinusCosTheta - uz * sinTheta;
    float r02 = ux * uz * oneMinusCosTheta + uy * sinTheta;

    float r10 = uy * ux * oneMinusCosTheta + uz * sinTheta;
    float r11 = cosTheta + uy * uy * oneMinusCosTheta;
    float r12 = uy * uz * oneMinusCosTheta - ux * sinTheta;

    float r20 = uz * ux * oneMinusCosTheta - uy * sinTheta;
    float r21 = uz * uy * oneMinusCosTheta + ux * sinTheta;
    float r22 = cosTheta + uz * uz * oneMinusCosTheta;
	
    float newX = r00 * p.x + r01 * p.y + r02 * p.z;
    float newY = r10 * p.x + r11 * p.y + r12 * p.z;
    float newZ = r20 * p.x + r21 * p.y + r22 * p.z;

    // return
    Point result
    result.x = newX;
    result.y = newY;
    result.z = newZ;
    return result;
}
void calc_orient(){ //use alpha beta to calculte orientation vectors, matices
	// horisontal rotation
	camera.forw = rotateAroundAxis(y_const, up_const, camera.beta);
	camera.rigth = rotateAroundAxis(x_const, up_const, camera.beta);
	
	//vertical
	camera.forw = rotateAroundAxis(camera.forw, camera.rigth, camera.alpha);
	camera.up = rotateAroundAxis(up_const, camera.rigth, camera.alpha);
}

void move(Direction dir){
	calc_orient();
	float ms = 0.3; //ms = movement speed
	switch (dir) {
    case LEFT:
        camera.pos += ms * (- camera.right);
        break;
    case RIGHT:
        camera.pos += ms * ( camera.right);
        break;
    case UP:
        camera.pos += ms * ( up_const );
        break;
    case DOWN:
        camera.pos += ms * ( -up_const );
        break;
    case FORWARD:
        camera.pos += ms * ( camera.forw);
        break;
    case BACKWARD:
        camera.pos += ms * (- camera.forw);
        break;
    default:
        
        break;
	}
}
void turn(Direction dir){
	float rs = 0.03;
	switch (dir) {
    case LEFT:
        camera.beta += rs;
        break;
    case RIGHT:
        camera.beta -= rs;
        break;
    case UP:
        camera.alpha += rs;
        break;
    case DOWN:
        camera.alpha -= rs;
        break;
    case FORWARD:
        
        break;
    case BACKWARD:
        
        break;
    default:
        
        break;
	}
	if(camera.alpha > M_PI/2 ) {
		camera.alpha = M_PI/2;
	}
	if(camera.alpha < -M_PI/2 ) {
		camera.alpha = -M_PI/2;
	}
	if(camera.beta > M_PI*2 ) {
		camera.beta -= M_PI*2;
	}
	if(camera.beta < 0 ) {
		camera.beta += M_PI*2;
	}
}

Pixel project_point(const Point& P){
	//depending on the current mode find coordinates on the drawing plane
	Pixel result;
	Point relative = P - camera.pos;
	
	float coord1, coord2;
	
	if(Pmode){
		float phi_hor = relative^camera.right;
		float phi_vert = relative^camera.up;
	
		//float coord1 = - phi_hor * (180/M_PI) * (DRAW_WIDTH / FOV) + DRAW_WIDTH/2;
		//float coord2 = - phi_vert * (180/M_PI) * (DRAW_WIDTH / FOV) + DRAW_WIDTH/2;
		float alpha = (180-FOV)/2;
		coord1 = (M_PI - phi_hor) * (180/M_PI) - alpha;
		coord2 = (phi_vert) * (180/M_PI) - alpha;
		
	} else {
		coord1 = relative*camera.right / len(camera.right);
		coord2 = relative*camera.up / len(camera.up);
		
		coord1 *= scaling;
		coord2 *= scaling;
	}
	int c1 = static_cast<int>(coord1);
	int c2 = static_cast<int>(coord2);
	result.x = c1;
	result.y = c2;
	result.color = P.color;
	return result;
}



void drawFace(Face face){
	Contour cont1 = FaceToContour(Face face);
	Segments f = convertContourToSegments(C);
	drawSegments(  f, main_color);
	
}

void drawScene(){
	//for each triangle call painter
	InitializeBuffer(); //clean color buffer
	resetZBuffer(); // clear depth buffer
	for (const int& face : scene) {
        drawFace(face);
    }
}

#endif // CAMERA_INCLUDED