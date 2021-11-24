#include "graphics.h"

#include "assets/mesh.h"
#include "assets/material.h"
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

void graphics_draw(mesh* mesh, material* mat) {
    // Set object rasterizer state.
    if (g_mode == FILL_MODE_SOLID) {
        d3d11_set_rasterizer_state(mat->mode);
    } else if (g_mode == FILL_MODE_WIREFRAME) {
        d3d11_set_rasterizer_state(RASTERIZER_WIREFRAME);
    }

    // Set constant buffer.
    if (mat->cb) {
        d3d11_vs_set_constant_buffer(mat->cb);
        d3d11_ps_set_constant_buffer(mat->cb);
    }

    // Set shaders.
    d3d11_set_vertex_shader((d3d11_vertex_shader*)mat->vs->ptr);
    d3d11_set_pixel_shader((d3d11_pixel_shader*)mat->ps->ptr);

    // Set texture.
    if (mat->texs_size) {
        d3d11_set_ps_texture((d3d11_texture**)mat->texs, mat->texs_size);
    }

    // Set vertex and index buffers.
    d3d11_set_vertex_buffer(mesh->vb);
    d3d11_set_index_buffer(mesh->ib);

    switch (mesh->topology) {
        case TOPOLOGY_TRIANGLE_LIST:
            d3d11_draw_indexed_triangle_list(mesh->ib->size_list, 0, 0);
        break;
        case TOPOLOGY_LINE_LIST:
            d3d11_draw_indexed_line_list(mesh->ib->size_list, 0, 0);
        break;
        default:
            warning("not set topology");
    }
}

void graphics_present() {
    // Swap buffers.
    d3d11_present(true);
}
