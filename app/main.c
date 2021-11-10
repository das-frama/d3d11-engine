#include <motor.h>

int main() {
	motor_init("assets", 1440, 900);

	// load assets
	asset* sphere = asset_load("sphere.obj");

	while(motor_running()) {
		frame_begin();

        motor_process_events();

        frame_end();
	}

	// unload assets
	asset_unload(sphere);

	// release engine resources
	motor_close();

	return 0;
}