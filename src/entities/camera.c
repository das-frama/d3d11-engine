#include "entities/camera.h"

#include "math/vec.h"
#include "math/mat.h"

#define pressed(key) in->keys[key].pressed
#define released(key) in->keys[key].released

camera camera_new(int w, int h) {
	camera cam = {0};

	cam.fov    = 1.25f;
	cam.znear  = 0.1f;
	cam.zfar   = 1000.f;

	cam.rot_x = 0.0f;
	cam.rot_y = 0.0f;

	cam.forward = 0.0f;
	cam.right   = 0.0f;
	cam.top     = 0.0f;

	win32_size(&cam.w, &cam.h);
	cam.world = mat4_translate(vec3_new(0, 0, 5));
	cam.view  = mat4_id();
	cam.proj  = mat4_id();

	return cam;
}

void camera_update(camera* cam, input* in, float dt) {
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

 		if (released('W') || released('S')) cam->forward = 0.0f;
		if (released('A') || released('D')) cam->right = 0.0f;
		if (released('Q') || released('E')) cam->top = 0.0f;
	}

    // Transform matrices.
    {
    	mat4 world_cam = mat4_mul_x(3, mat4_id(), mat4_rotate_x(cam->rot_x), mat4_rotate_y(cam->rot_y - 4.5f));
		// mat4 world_cam = mat4_id();
    	vec3 z_dir = vec3_mul(mat4_z_direction(world_cam), cam->forward * 0.2f);
    	vec3 x_dir = vec3_mul(mat4_x_direction(world_cam), cam->right * 0.2f);
    	vec3 y_dir = vec3_mul(mat4_y_direction(mat4_id()), cam->top * 0.2f);
    	vec3 new_pos = vec3_add_x(4, mat4_translation(cam->world), z_dir, x_dir, y_dir);
    	// vec3 new_pos = vec3_add_x(4, mat4_translation(cam->world), z_dir, x_dir, y_dir);
    	world_cam = mat4_translate_mat4(world_cam, new_pos);
    	
    	cam->world = world_cam;
    	cam->view = mat4_inverse(world_cam);
    	cam->proj = mat4_perspective(cam->fov, (float)cam->w / (float)cam->h, cam->znear, cam->zfar);
    }
}