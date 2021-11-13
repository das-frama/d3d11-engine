#include "graphics.h"

#include "platform/win32_platform.h"
#include "renderer/d3d11_renderer.h"

void graphics_init(int w, int h) {
	// Init win32.
	win32_init();

    // Default values.
	win32_create(w, h);
    win32_show(true);

    // Init d3d11.
    d3d11_init(w, h);

    // Set vieport size.
    d3d11_set_viewport_size(w, h);
}

void graphics_close() {
    d3d11_close();
    win32_close();
}

void graphics_window_size(int *w, int *h) {
    win32_size(w, h);
}

void graphics_window_resize(int w, int h) {
    win32_resize(w, h);
    d3d11_set_viewport_size(w, h);
}

void graphics_window_set_title(const char* title) {
    win32_set_title(title);
}

void graphics_do_one_frame() {
    frame_begin();

    graphics_present();
    
    frame_end();
}

void graphics_clear_screen(float r, float g, float b, float a) {
    d3d11_clear_render_target_color(r, g, b, a);
}

void graphics_draw(mesh* m, material* mat) {
    d3d11_set_rasterizer_state(mat->cull_mode == CULL_MODE_FRONT);

    // Set constant buffer.
    d3d11_vs_set_const_buffer(mat->cb);
    d3d11_ps_set_const_buffer(mat->cb);

    // Set shaders.
    d3d11_set_vertex_shader(mat->vs);
    d3d11_set_pixel_shader(mat->ps);

    // Set texture.
    d3d11_set_ps_texture(mat->texs, mat->texs_size);

    // Set vertex and index buffers.
    d3d11_set_vertex_buffer(m->vb);
    d3d11_set_index_buffer(m->ib);

    // Draw.
    d3d11_draw_indexed_triangle_list(m->ib->size_list, 0, 0);
}

void graphics_present() {
    // Swap buffers.
    d3d11_present(true);
}

// const_buffer graphics_create_const_buffer(size_t buffer_size) {
//     return d3d11_create_const_buffer(buffer_size);
// }

// void graphics_update_const_buffer(const_buffer* cb, void* buffer) {
//     d3d11_update_const_buffer(cb, buffer);
// }