#include "entities/geometry.h"

#include "shaders/grid_vs.h"
#include "shaders/grid_ps.h"
#include "renderer/d3d11_renderer.h"

typedef struct {
    vec3 pos;
    vec4 color;
} vertex_grid;

grid* grid_new(float w, float d, int m, int n, vec4 color) {
    grid* g = malloc(sizeof(grid));
    memset(g, 0, sizeof(grid));
    memset(&g->cc, 0, sizeof(constant_grid));

    int vertex_count = (m + n) * 2;
    int index_count = (m+1 + n+1) * 2;

    float half_width = 0.5f * w;
    float half_depth = 0.5f * d;

    float dx = w / n;
    float dz = d / m;

    // Generate vertices.
    vertex_grid* vertices = malloc(sizeof(vertex_grid) * vertex_count);
    for (int i = 0; i <= m; i++) {
        float x =  -half_width + i * dx;
        vertices[i].pos = vec3_new(x, 0, half_depth);
        vertices[i].color = color;
        vertices[m+n+i].pos = vec3_new(-x, 0, -half_depth);;
        vertices[m+n+i].color = color;
    }
    for (int i = 0; i <= n; i++) {
        float z = half_depth - i * dz;
        vertices[m+i].pos = vec3_new(half_width, 0, z);
        vertices[m+i].color = color;
        vertices[(vertex_count-n+i) % vertex_count].pos = vec3_new(-half_width, 0, -z);
        vertices[(vertex_count-n+i) % vertex_count].color = color;
    }

    // Generate indices.
    uint* indices = malloc(sizeof(uint) * index_count);
    size_t index = 0;
    for (int i = 0; i <= m; i++) {
        indices[index] = i;
        indices[index+1] = vertex_count-m-i;
        index +=2;
    }
    for (int i = 0; i <= n; i++) {
        indices[index] = (vertex_count-i) % vertex_count;
        indices[index+1] = n+i;
        index +=2;
    }

    // Create index and vertex buffers.
    {
        g->mesh = malloc(sizeof(mesh));
        memset(g->mesh, 0, sizeof(mesh));
        g->mesh->vb = d3d11_create_grid_vertex_buffer(
            vertices, sizeof(vertex_grid), vertex_count, (void*)g_grid_vs, _countof(g_grid_vs)
        );
        g->mesh->ib = d3d11_create_index_buffer(indices, index_count);
    }

    {
        g->material = malloc(sizeof(material));
        memset(g->material, 0, sizeof(material));
        g->material->vs = shader_load("..\\assets\\grid_vs.hlsl", "vsmain", SHADER_TYPE_VS);
        g->material->ps = shader_load("..\\assets\\grid_ps.hlsl", "psmain", SHADER_TYPE_PS);
        g->material->cb = d3d11_create_const_buffer(&g->cc, sizeof(constant_grid));
    }

    return g;
}

void grid_delete(grid* g) {
    mesh_unload(g->mesh);
    material_unload(g->material);

    free(g);
}

void grid_update(grid* g, const camera* cam) {
    g->cc.world = mat4_id();
    g->cc.view  = cam->view;
    g->cc.proj  = cam->proj;
    d3d11_update_const_buffer(g->material->cb, &g->cc);
}

void grid_draw(const grid* g) {
    // Set constant buffer.
    d3d11_vs_set_const_buffer(g->material->cb);
    d3d11_ps_set_const_buffer(g->material->cb);

    // Set shaders.
    d3d11_set_vertex_shader(g->material->vs);
    d3d11_set_pixel_shader(g->material->ps);

    // Vertex and index buffers.
    d3d11_set_vertex_buffer(g->mesh->vb);
    d3d11_set_index_buffer(g->mesh->ib);

    // Draw.
    d3d11_draw_indexed_line_list(g->mesh->ib->size_list, 0, 0);
}