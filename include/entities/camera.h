#ifndef MOTOR_CAMERA_H
#define MOTOR_CAMERA_H

#include "engine.h"

#include "entity.h"

// typedef struct {
// 	mat4 world, view, proj;

// 	float fov;
// 	float znear, zfar;

// 	float rot_x, rot_y;
// 	float forward, right, top;

// 	float speed;

// 	int w, h;

// 	int index;
// } camera;

entity_id camera_new(float x, float y, float z);
void camera_delete(entity_id id);
void camera_update(camera* cam);
// void camera_delete_all(void);

// camera* camera_get(size_t index);
// void camera_update_all(float dt);

#endif // MOTOR_CAMERA_H