#include "motor.h"

extern bool win32_should_quit;

void motor_init(const char* name, int w, int h) {
	log("Starting motor...");

	log("Init graphics...");
	graphics_init(w, h);
    graphics_window_set_title(name);

    win32_show_mouse(false);

    //log("Init assets...");
    //assets_init(core_assets_path);
}

void motor_close() {
    log("Closing motor...");

    log("Closing graphics...");
    graphics_close();

    // log("Closing assets...");
    // assets_close();
}

bool motor_running() {
    return !win32_should_quit;
}

void motor_stop_next() {
    win32_should_quit = true;
}