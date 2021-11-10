#include "graphics.h"

#include "platform/win32_platform.h"
#include "renderer/d3d11_renderer.h"

renderer* rnd = NULL;
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

    // Set vieport size.
    d3d11_set_viewport_size(rnd, w, h);

    // Set default rasterizer state.
    d3d11_set_rasterizer_state(rnd, false);
}

void graphics_close() {
    log("closing d3d11...");
    d3d11_close(rnd);

    log("closing win32...");
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

void graphics_clear_screen(float r, float g, float b, float a) {
    d3d11_clear_render_target_color(rnd, r, g, b, a);
}

void graphics_draw_mesh(mesh* m, shader* s) {
    // Set constant buffer.

    // Set shaders.
    d3d11_set_pixel_shader(rnd,  &s->ps);
    d3d11_set_vertex_shader(rnd, &s->vs);

    // Set texture.

    // Set vertex and index buffers.
    d3d11_set_vertex_buffer(rnd, &m->vb);
    d3d11_set_index_buffer(rnd,  &m->ib);

    // Draw.
    d3d11_draw_indexed_triangle_list(rnd, m->ib.size_list, 0, 0);
}

void graphics_present() {
    // Swap buffers.
    d3d11_present(rnd, true);
}
