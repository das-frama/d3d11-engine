#include "entities/camera.h"

camera* camera_new(void) {
	camera* c = malloc(sizeof(camera));

	c->pos = vec3_new(10, 10, 10);
	c->target = vec3_zero();
	c->fov = 1.25f;
	c->znear = 0.1f;
	c->zfar = 100.0f;

	return c;
}

void camera_delete(camera* c) {
	free(c);
}

void camera_update(camera* c) {
	
}
