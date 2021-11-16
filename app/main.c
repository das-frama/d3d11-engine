#include <motor.h>

#include "planet.c" // yes, it's a unity build!

#define HEIGHT 1080
#define WIDTH  1920

#define PLANET_SIZE 4

int main() {
	// init engine
	motor_init("assets", "Solar system", WIDTH, HEIGHT);

	// Engine resources.
	input in = {0};

	// Engine entities.
	camera* cam = camera_new(0, 50, 0);
	grid* grid = grid_new(1024, 1024, 64, 64, vec4_new(.5f, .5f, .5f, 1));

	// Create engine objects.
	planet* planets[PLANET_SIZE];
	planets[0] = planet_new(0.0f,  20.0f, 0.0f, 0.0f, 0.0f, "assets\\textures\\sun.jpg");
	planets[1] = planet_new(30.0f, 1.0f,  1.0f, 0.0f, 0.0171f, "assets\\textures\\mercury.jpg");
	planets[2] = planet_new(42.0f, 0.39f, 2.85f, -177.4f, 0.0041f, "assets\\textures\\venus.jpg");
	planets[3] = planet_new(55.0f, 3.0f, 0.24f, 0.0f, 1.0f, "assets\\textures\\earth_day.jpg");

	// Game loop.
    float planet_time = 0.0f;
	while(motor_running()) {
		frame_begin();

		// Platform events.
		platform_events();

		// Input events.
		input_update(&in);
		if (in.keys[27].pressed) {
			motor_stop_next();
		}

		// Update.
		camera_update(cam, &in, frame_dt());
		grid_update(grid, cam);
		for (int i = 0; i < PLANET_SIZE; i++) {
			planet_update(planets[i], planet_time);
		}
		planet_time += 0.005f;

		// Render.
        graphics_clear_screen(0.0f, 0.0f, 0.0f, 0.0f);
        grid_draw(grid);
        for (int i = 0; i < PLANET_SIZE; i++) {
        	planet_draw(planets[i], cam);
		}
        graphics_present();

		frame_end();
	}

	// release application assets.
	for (int i = 0; i < PLANET_SIZE; i++) {
		planet_delete(planets[i]);
	}
	planet_delete_static();

	// release engine resources.
	grid_delete(grid);
	camera_delete(cam);
	motor_close();

	return 0;
}