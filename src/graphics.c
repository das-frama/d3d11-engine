#include "graphics.h"

#include "renderer/d3d11_renderer.h"

static fill_mode g_mode = FILL_MODE_SOLID;

void graphics_init(int w, int h) {
    // Init d3d11.
    d3d11_init(w, h);

    // Set vieport size.
    d3d11_set_viewport_size(w, h);
}

void graphics_close() {
    d3d11_close();
}

void graphics_clear(float r, float g, float b, float a) {
    d3d11_clear_render_target_color(r, g, b, a);
}

void graphics_draw(renderable* r) {
    // Set object rasterizer state.
    if (g_mode == FILL_MODE_SOLID) {
        d3d11_set_rasterizer_state(r->material->mode);
    } else if (g_mode == FILL_MODE_WIREFRAME) {
        d3d11_set_rasterizer_state(RASTERIZER_WIREFRAME);
    }

    // Set constant buffer.
    if (r->material->cb) {
        d3d11_vs_set_constant_buffer(r->material->cb);
        d3d11_ps_set_constant_buffer(r->material->cb);
    }

    // Set shaders.
    d3d11_set_vertex_shader((d3d11_vertex_shader*)r->material->vs->ptr);
    d3d11_set_pixel_shader((d3d11_pixel_shader*)r->material->ps->ptr);

    // Set texture.
    if (r->material->texs_size) {
        d3d11_set_ps_texture((d3d11_texture**)r->material->texs, r->material->texs_size);
    }

    // Set vertex and index buffers.
    for (int i = 0; i < r->mesh_size; i++) {
        d3d11_set_vertex_buffer(r->meshes[i]->vb);
        d3d11_set_index_buffer(r->meshes[i]->ib);
        d3d11_draw_indexed_triangle_list(r->meshes[i]->ib->size_list, 0, 0);
    }
}

void graphics_present() {
    // Swap buffers.
    d3d11_present(true);
}
