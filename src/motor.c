#include "motor.h"

void motor_init(const char* title, int w, int h) {
    log("Init window...");
    window_init();
    window_create(w, h);

	log("Init graphics...");
	graphics_init(w, h);

    // Setup window.
    log("Show window...");
    window_set_title(title);
    window_show(true);

    input_set_screen_area(window_screen_area());

    log("Init entity system...");
    entity_init();

    game_start();
}

void motor_close() {
    log("Closing entity system...");
    entity_close();

    log("Closing motor...");
    game_stop();

    log("Closing graphics...");
    graphics_close();

    log("Closing window...");
    window_close();
}