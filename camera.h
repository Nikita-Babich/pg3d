

#ifndef _CAMERA_INCLUDED_
#define _CAMERA_INCLUDED_

#ifndef _STRUCTS_INCLUDED_
#include "structs.h"
#endif


//there is only one camera at any instance, so instead of functions procedures are used here
// Camera should change in place to be the global object
typedef struct {	// 
    Point pos;          //position
    float beta; 		//horisontal angle
    float alpha;		//vertical angle, there is no roll
    
    // result of orientation calculation, 3 relative vectors
    Point forw;   //from y
    Point right; //from x
    Point up; //from z
} Camera;

Camera camera;
camera.pos = {0,0,0};
camera.beta = 0;
camera.alpha = 0;

Point up_const = {0,0,1};

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


#endif // CAMERA_INCLUDED