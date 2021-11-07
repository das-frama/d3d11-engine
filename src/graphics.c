#include "graphics.h"

static HWND window;

void graphics_init(int w, int h) {
	log("win32 initialization...");
	// Init win32.
	bool ok = win32_init();
	if (!ok) {
		error("win32 initialization error!");
	}
	window = win32_create_window(w, h);
    win32_show_window(window, true);

    // Init d3d11.
    log("d3d11 initialization...");
}

void graphics_close() {
    // d3d11_close();
    win32_close(window);
}

void graphics_window_update() {
    win32_broadcast();
}

HWND graphics_get_window() {
    return window;
}

void graphics_get_window_size(int *w, int *h) {
    win32_get_window_size(window, w, h);
}
