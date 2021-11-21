#include "entities/grid.h"

#include "utils/geometry.h"
#include "renderer/d3d11_renderer.h"
#include "shaders/grid_vs.h"
#include "shaders/grid_ps.h"

grid* grid_new(float w, float d, int m, int n, vec4 color) {
    grid* g = calloc(1, sizeof(grid));
    memset(&g->cc, 0, sizeof(constant_grid));
    g->cc.color = color;

    // Create mesh.
    {
        mesh_data* md = generate_grid(w, d, m, n);
        g->mesh = calloc(1, sizeof(mesh));

        g->mesh->vb = d3d11_create_vertex_buffer(
            md->vertices, sizeof(vertex), md->vertices_count, (void*)g_grid_vs, _countof(g_grid_vs)
        );
        g->mesh->ib = d3d11_create_index_buffer(md->indices, md->indices_count);
        mesh_data_delete(md);
    }

    // Create material.
    {
        g->material = calloc(1, sizeof(material));
        
        g->material->vs = shader_new();
        g->material->vs->type = SHADER_TYPE_VS;
        shader_set_byte(g->material->vs, (void*)g_grid_vs, _countof(g_grid_vs));
        g->material->ps = shader_new();
        g->material->ps->type = SHADER_TYPE_PS;
        shader_set_byte(g->material->ps, (void*)g_grid_ps, _countof(g_grid_ps));

        g->material->cb = d3d11_create_constant_buffer(&g->cc, sizeof(constant_grid));
    }

    return g;
}

void grid_delete(grid* g) {
    mesh_delete(g->mesh);
    material_delete(g->material);

    free(g);
}

void grid_update(grid* g, const camera* cam) {
    g->cc.world = mat4_id();
    g->cc.view  = cam->view;
    g->cc.proj  = cam->proj;
    d3d11_update_constant_buffer(g->material->cb, &g->cc);
}

void grid_draw(const grid* g) {
    d3d11_set_rasterizer_state(CULL_MODE_BACK);
    // Set constant buffer.
    d3d11_vs_set_constant_buffer(g->material->cb);
    d3d11_ps_set_constant_buffer(g->material->cb);

    // Set shaders.
    d3d11_set_vertex_shader((d3d11_vertex_shader*)g->material->vs->ptr);
    d3d11_set_pixel_shader((d3d11_pixel_shader*)g->material->ps->ptr);

    // Vertex and index buffers.
    d3d11_set_vertex_buffer(g->mesh->vb);
    d3d11_set_index_buffer(g->mesh->ib);

    // Draw.
    d3d11_draw_indexed_line_list(g->mesh->ib->size_list, 0, 0);
}