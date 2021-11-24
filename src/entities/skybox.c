#include "entities/skybox.h"

#include "utils/geometry.h"
#include "shaders/skybox_vs.h"
#include "shaders/skybox_ps.h"

entity_id skybox_new() {
    entity_id id = entity_create();
    entity_add_transform(id, vec3_zero(), vec3_new(1, 1, 1), quat_id());


    // Generate and set sphere mesh.
    mesh_data* md = generate_sphere(1000.0f, 30, 30);
    mesh* mesh = mesh_new_data(md);
    mesh_data_delete(md);

    // Set default material.
    material* mat = material_new();
    mat->vs = shader_new_byte((void*)g_skybox_vs, _countof(g_skybox_vs), SHADER_TYPE_VS);
    mat->ps = shader_new_byte((void*)g_skybox_ps, _countof(g_skybox_ps), SHADER_TYPE_PS);
    mat->mode = CULL_MODE_FRONT;

    entity_add_renderable(id, mesh, mat);

    return id;
}

void skybox_delete(entity_id id) {
    entity_delete_renderable(id);
    entity_delete_transform(id);
}
