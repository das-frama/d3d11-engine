#include "entities/skybox.h"

#include "utils/geometry.h"
#include "shaders/skybox_vs.h"
#include "shaders/skybox_ps.h"

skybox* skybox_new() {
    skybox* sky = calloc(1, sizeof(skybox));
    sky->cc.color1 = color(0.878f, 0.918f, 0.988f, 1); // #CFDEF3
    sky->cc.color = color(0.812f, 0.871f, 0.891f, 1); // #E0EAFC

    sky->rnd = renderable_new();

    // Generate and set sphere mesh.
    mesh_data* md = generate_sphere(1000.0f, 30, 30);
    mesh* msh = mesh_new_data(md);
    mesh_data_delete(md);
    renderable_add_mesh(sky->rnd, msh);

    // Set default material.
    shader* vs = shader_new_byte(g_skybox_vs, _countof(g_skybox_vs), SHADER_TYPE_VS);
    shader* ps = shader_new_byte(g_skybox_ps, _countof(g_skybox_ps), SHADER_TYPE_PS);
    sky->rnd->material = material_new_(vs, ps, NULL, &sky->cc, sizeof(skybox_constant));
    sky->rnd->material->mode = CULL_MODE_FRONT;

    return sky;
}

void skybox_delete(skybox* sky) {
    renderable_delete_all(sky->rnd);
    free(sky);
}

void skybox_update(skybox* sky, const camera* cam) {
    sky->cc.world = mat4_id();
    sky->cc.view  = cam->view;
    sky->cc.proj  = cam->proj;
    d3d11_update_constant_buffer(sky->rnd->material->cb, &sky->cc);
}

void skybox_draw(const skybox* sky) {
    graphics_draw(sky->rnd);
}