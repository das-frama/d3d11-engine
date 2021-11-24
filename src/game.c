#include "game.h"

#include "input.h"
#include "window.h"
#include "graphics.h"

extern bool win32_should_quit;
static bool g_play_mode = false;

void game_start() {
    game_set_playmode(true);    
}

void game_pause() {
    // todo
}

void game_stop() {
    game_set_playmode(false);
}

bool game_should_quit() {
    return win32_should_quit;
}

void game_should_stop_next() {
    win32_should_quit = true;
}

void game_set_playmode(bool mode) {
    g_play_mode = mode;

    input_enable(g_play_mode);
    window_show_mouse(!g_play_mode);    
}

void game_switch_playmode() {
    game_set_playmode(!g_play_mode);
}

void game_do_one_frame() {
    frame_begin();

    // Update window events.
    window_events();

    // Update input.
    input_update();

    // Draw.
    graphics_clear(0.0f, 0.0f, 0.0f, 1.0f);
    graphics_present();

    frame_end();
}