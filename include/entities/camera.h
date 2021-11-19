#ifndef MOTOR_CAMERA_H
#define MOTOR_CAMERA_H

#include "engine.h"
#include "input.h"

typedef struct {
	mat4 world, view, proj;

	float fov;
	float znear, zfar;

	float rot_x, rot_y;
	float forward, right, top;

	float speed;

	int w, h;
} camera;

camera* camera_new(float x, float y, float z);
void camera_delete(camera* cam);

void camera_update(camera* cam, input* in, float dt);

#endif // MOTOR_CAMERA_H