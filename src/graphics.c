#include "graphics.h"

#include "platform/win32_platform.h"
#include "renderer/d3d11_renderer.h"

static renderer* rnd = NULL;
HWND window = {0};

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
    rnd = d3d11_init(w, h);
}

void graphics_close() {
    log("closing d3d11...");
    d3d11_close(rnd);

    log("closing win32...");
    win32_close(window);
}

void graphics_window_update() {
    win32_broadcast();

    d3d11_present(rnd, true);
}

HWND graphics_get_window() {
    return window;
}

void graphics_get_window_size(int *w, int *h) {
    win32_get_window_size(window, w, h);
}