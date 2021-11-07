#include <motor.h>

int main() {
	motor_init("Hello", 1920, 1080);

	while(motor_running()) {
		motor_begin_frame();
        motor_process_events();
        motor_end_frame();
	}

	motor_close();

	return EXIT_SUCCESS;
}
