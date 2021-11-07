#include "motor.h"

bool running = false;

void motor_init(const char* core_asset_path, int w, int h) {
	log("Starting motor...");

	log("Init graphics...");
	graphics_init(w, h);

	running = true;
}

bool motor_running() {
    return running;
}

void motor_process_events() {
    graphics_window_update();
}

void motor_close() {
    printf("Closing engine...\n");

    printf("Closing graphics manager...\n");
    graphics_close();
}

static char frame_rate_string_var[12];

static s32 frame_rate_var = 0;
static f32 frame_delta_time = 0.0f;

static u64 frame_start_time = 0;
static u64 frame_end_time = 0;

static const f32 frame_update_rate = 0.5f;

static u16 frame_counter = 0;
static f32 frame_acc_time = 0.0f;

void motor_begin_frame() {
    frame_start_time = GetTickCount64();
}

void motor_end_frame() {
    frame_end_time = GetTickCount64();

    frame_delta_time = ((f32) (frame_end_time - frame_start_time) / 1000.0f);
    frame_acc_time += frame_delta_time;
    frame_counter++;

    if (frame_acc_time > frame_update_rate) {
        frame_rate_var = round((f32) frame_counter / frame_acc_time);
        frame_counter = 0;
        frame_acc_time = 0.0f;
    }

    sprintf(frame_rate_string_var, "%i", frame_rate_var);
//    printf("%s\n", frame_rate_string_var);
}