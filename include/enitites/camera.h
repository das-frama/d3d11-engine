#ifndef MOTOR_CAMERA_H
#define MOTOR_CAMERA_H

#include "engine.h"

typedef struct {
	vec3 pos;
	vec3 target;
	float fov;
	float znear;
	float zfar;
} camera;

camera* camera_new(void);
void camera_delete(camera* c);

void camera_update(camera* c);

#endif // MOTOR_CAMERA_H