

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
camera.beta=0;
camera.alpha=0;



#endif // CAMERA_INCLUDED