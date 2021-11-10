#include <motor.h>

int main() {
	motor_init("assets", 1440, 900);

	// load assets
	asset* sphere_obj	 = asset_load("sphere.obj");
	asset* sphere_shader = asset_load("sphere.fx");

	while(motor_running()) {
		frame_begin();

        motor_process_events();

        graphics_clear_screen(0.42f, 0.92f, 0.25f, 1.0f);
        graphics_draw_mesh(sphere_obj->ptr, sphere_shader->ptr);
        graphics_present();

        frame_end();
	}

	// unload assets
	asset_unload(sphere_shader);
	asset_unload(sphere_obj);

	// release engine resources
	motor_close();

	return 0;
}