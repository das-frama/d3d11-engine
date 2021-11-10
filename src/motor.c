#include "motor.h"

bool running = false;

void motor_init(const char* core_asset_path, int w, int h) {
	log("Starting motor...");

	log("Init graphics...");
	graphics_init(w, h);

    log("Init assets...");
    asset_init(core_asset_path);

	running = true;
}

void motor_close() {
    running = false;
    log("Closing engine...\n");

    log("Closing graphics manager...\n");
    graphics_close();
}

bool motor_running() {
    return running;
}

void motor_process_events() {
    graphics_window_update();
}
