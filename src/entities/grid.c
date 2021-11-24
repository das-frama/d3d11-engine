#include "entities/grid.h"

#include "assets/mesh.h"
#include "assets/material.h"
#include "utils/geometry.h"
#include "renderer/d3d11_renderer.h"
#include "shaders/grid_vs.h"
#include "shaders/grid_ps.h"

entity_id grid_new(int width, int depth, int m, int n) {
    // grid* new_grid = calloc(1, sizeof(grid));
    entity_id id = entity_create();
    entity_add_transform(id, vec3_zero(), vec3_new(1, 1, 1), quat_id());

    // Create mesh.
    mesh_data* md = generate_grid(width, depth, m, n);
    mesh* mesh = mesh_new_data(md);
    mesh->topology = TOPOLOGY_LINE_LIST;
    mesh_data_delete(md);

    // Create material.
    material* mat = material_new();
    mat->vs = shader_new_byte((void*)g_grid_vs, _countof(g_grid_vs), SHADER_TYPE_VS);
    mat->ps = shader_new_byte((void*)g_grid_ps, _countof(g_grid_ps), SHADER_TYPE_PS);
    mat->mode = CULL_MODE_BACK;

    entity_add_renderable(id, mesh, mat);

    return id;
}

void grid_delete(entity_id id) {
    entity_delete_renderable(id);
    entity_delete_transform(id);
}