#include <motor.h>

#include "planet.c" // yes, it's a unity build!

#define HEIGHT 1080
#define WIDTH  1920

int main() {
	// init engine
	motor_init("assets", "Solar system", WIDTH, HEIGHT);

	// Engine resources.
	input in = {0};

	// Engine entities.
	camera* cam = camera_new();
	grid* grid = grid_new(1024, 1024, 64, 64, vec4_new(.5f, .5f, .5f, 1));

	// Create engine objects.
	planet* sun = planet_new(0.0f, 5.0f);

	// Game loop.
	while(motor_running()) {
		frame_begin();

		// Platform events.
		platform_events();

		// Input events.
		input_update(&in);

		// Update.
		camera_update(cam, &in, frame_dt());
		grid_update(grid, cam);
		planet_update(sun, cam, frame_dt());

		// Render.
        graphics_clear_screen(0.0f, 0.0f, 0.0f, 1.0f);
        grid_draw(grid);
        planet_draw(sun);
        graphics_present();

		frame_end();
	}

	// release application assets.
	planet_delete(sun);

	// release engine resources.
	grid_delete(grid);
	camera_delete(cam);
	motor_close();

	return 0;
}