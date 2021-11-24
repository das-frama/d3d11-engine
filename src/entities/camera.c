#include "entities/camera.h"

#include "input.h"
#include "window.h"

entity_id camera_new(float x, float y, float z) {
	entity_id id = enity_create();

	entity_add_transform(id, vec3_zero(), vec3_zero(), quat_id());
	entity_add_movement(id, vec3_zero());
	entity_add_camera(id, 1.25f, 0.1f, 1000.0f);

	return id;
}

void camera_delete(entity_id id) {
	entity_delete_transform(id);
	entity_delete_movement(id);
	entity_delete_camera(id);
}