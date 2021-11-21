#include "window.h"

#include "platform/win32_platform.h"

void window_init() {
    // Init win32.
    win32_init();
}

void window_close() {
    win32_close();
}

void window_create(int w, int h) {
     // Create and show window.
    win32_create(w, h);
}

void window_show(bool show) {
    win32_show(show);
}

void window_size(int *w, int *h) {
    win32_size(w, h);
}

void window_resize(int w, int h) {
    win32_resize(w, h);
    d3d11_set_viewport_size(w, h);
}

void window_set_title(const char* title) {
    win32_set_title(title);
}

void window_show_mouse(bool show) {
    win32_show_mouse(show);
}

void window_set_mouse_pos(float x, float y) {
    win32_set_mouse_pos(x, y);
}

void window_events() {
    win32_process_messages();
}