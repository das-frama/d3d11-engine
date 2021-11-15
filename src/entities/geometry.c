#include "entities/geometry.h"

#include "shaders/grid_vs.h"
#include "shaders/grid_ps.h"
#include "renderer/d3d11_renderer.h"

typedef struct {
    vec3 pos;
    vec4 color;
} vertex_grid;

grid generate_grid(float w, float d, int m, int n, vec4 color) {
    grid g = {0};
    memset(&g, 0, sizeof(grid));

    int vertex_count = m * n;
    int index_count = (m-1)*(n-1)*6;

    float half_width = 0.5f * w;
    float half_depth = 0.5f * d;

    float dx = w / (n-1);
    float dz = d / (m-1);

    // Generate vertices.
    vertex_grid* vertices = malloc(sizeof(vertex_grid) * vertex_count);
    for (int i = 0; i < m; i++) {
        float z = half_depth - i*dz;
        for (int j = 0; j < n; j++) {
            float x = -half_width + j*dx;
            vertices[i*n+j].pos = vec3_new(x, 0, z);
            vertices[i*n+j].color = color;
        }
    }

    // Generate indices.
    uint* indices = malloc(sizeof(uint) * index_count);
    int k = 0; 
    for (int i = 0; i < m-1; i++) {
        for (int j = 0; j < n-1; j++) {
            
            

            // Quads.
            // indices[k]   = i*n+j;
            // indices[k+1] = i*n+(j+1);
            // indices[k+2] = (i+1)*n+j;

            // indices[k+3] = i*n+(j+1);
            // indices[k+5] = (i+1)*n+(j+1);
            // indices[k+4] = (i+1)*n+j;
            // k += 6; // next quad.
        }
    }

    // Create index and vertex buffers.
    g.m.vb = d3d11_create_grid_vertex_buffer(
        vertices, sizeof(vertex_grid), vertex_count, (void*)g_grid_vs, _countof(g_grid_vs)
    );
    g.m.ib = d3d11_create_index_buffer(indices, index_count);

    g.mat.vs = shader_load("..\\assets\\grid_vs.hlsl", "vsmain", SHADER_TYPE_VS);
    g.mat.ps = shader_load("..\\assets\\grid_ps.hlsl", "psmain", SHADER_TYPE_PS);
    g.mat.cb = d3d11_create_const_buffer(&g.cc, sizeof(constant_grid));

    return g;
}

void grid_update(grid* g, const camera* cam) {
    g->cc.world = mat4_id();
    g->cc.view  = cam->view;
    g->cc.proj  = cam->proj;
    d3d11_update_const_buffer(g->mat.cb, &g->cc);
}