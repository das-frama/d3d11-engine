#include "entities/camera.h"

#include "input.h"
#include "window.h"

#include "math/vec.h"
#include "math/mat.h"

#include "vendor/stb_ds.h"

#define pressed(key) in->keys[key].pressed
#define released(key) in->keys[key].released

static camera** cameras = NULL;

camera* camera_new(float x, float y, float z) {
	camera* cam = calloc(1, sizeof(camera));

	cam->fov    = 1.25f;
	cam->znear  = 0.1f;
	cam->zfar   = 1100.f;

	cam->rot_x = 0.0f;
	cam->rot_y = 0.0f;

	cam->forward = 0.0f;
	cam->right   = 0.0f;
	cam->top     = 0.0f;

	cam->speed = 0.2f;

	window_size(&cam->w, &cam->h);
	cam->world = mat4_translate(vec3_new(x, y, z));
	cam->view  = mat4_id();
	cam->proj  = mat4_id();

	arrput(cameras, cam);
	cam->index = arrlen(cameras) - 1;

	return cam;
}

void camera_delete(camera* cam) {
	arrdel(cameras, cam->index);
	free(cam);
}

void camera_delete_all() {
	for (size_t i = 0; i < arrlen(cameras); i++) {
        free(cameras[i]);
    }

    arrfree(cameras);
}

void camera_update(camera* cam, float dt) {
	input* in = input_get();

	// Change camera orbit.
	{
		if (in->mouse.has_movement) {
			cam->rot_x += (in->mouse.y - cam->h / 2.0f) * dt * 0.1f;
	    	cam->rot_y += (in->mouse.x - cam->w / 2.0f) * dt * 0.1f;
		}
	}

	// Change camera position.
	{
		if (pressed('W')) 	    cam->forward = 1.0f;
		else if (pressed('S')) 	cam->forward = -1.0f;

		if (pressed('A')) 		cam->right = -1.0f;
		else if (pressed('D'))  cam->right = 1.0f;

		if (pressed('Q')) 		cam->top = -1.0f;
		else if (pressed('E')) 	cam->top = 1.0f;

		if (pressed(VK_SHIFT))  cam->speed = 2.0f;

 		if (released('W') || released('S')) cam->forward = 0.0f;
		if (released('A') || released('D')) cam->right = 0.0f;
		if (released('Q') || released('E')) cam->top = 0.0f;
		if (released(VK_SHIFT)) cam->speed = 0.2f;
	}

    // Transform matrices.
    {
    	mat4 world_cam = mat4_mul_x(3, mat4_id(), mat4_rotate_x(cam->rot_x), mat4_rotate_y(cam->rot_y - 4.5f));
    	vec3 z_dir = vec3_mul(mat4_z_direction(world_cam), cam->forward * cam->speed);
    	vec3 x_dir = vec3_mul(mat4_x_direction(world_cam), cam->right * cam->speed);
    	vec3 y_dir = vec3_mul(mat4_y_direction(mat4_id()), cam->top * cam->speed);
    	vec3 new_pos = vec3_add_x(4, mat4_translation(cam->world), z_dir, x_dir, y_dir);
    	world_cam = mat4_translate_mat4(world_cam, new_pos);
    	
    	cam->world = world_cam;
    	cam->view = mat4_inverse(world_cam);
    	cam->proj = mat4_perspective(cam->fov, (float)cam->w / (float)cam->h, cam->znear, cam->zfar);
    }
}

void camera_update_all(float dt) {
	for (size_t i = 0; i < arrlen(cameras); i++) {
        camera_update(cameras[i], dt);
    }
}