#include <motor.h>

int main() {
	motor_init("Hello", 1440, 900);

	// asset_load();

	while(motor_running()) {
		frame_begin();

        motor_process_events();

        frame_end();
	}

	motor_close();

	return 0;
}