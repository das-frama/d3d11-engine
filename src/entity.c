#include "entity.h"
#include "graphics.h"
#include "input.h"

#include "assets/mesh.h"
#include "assets/material.h"
#include "vendor/stb_ds.h"

#define pressed(key) in->keys[key].pressed
#define released(key) in->keys[key].released

static component_list g_components = {0};
static entity_id g_entity_id = 0;

void entity_init() {
    g_components.transforms = NULL;
    g_components.movements = NULL;
    g_components.renderables = NULL;
    g_components.cameras = NULL;
    g_components.players = NULL;
}

void entity_close() {
    hmfree(g_components.transforms);
    hmfree(g_components.movements);
    hmfree(g_components.renderables);
    hmfree(g_components.cameras);
    hmfree(g_components.players);
}

entity_id entity_create() {
    entity_id id = g_entity_id++;
    return id;
}

void entity_remove(entity_id id) {
    hmdel(g_components.transforms, id);
    hmdel(g_components.movements, id);
    hmdel(g_components.renderables, id);
    hmdel(g_components.cameras, id);
    hmdel(g_components.players, id);
}

void entity_add_transform(entity_id id, vec3 pos, vec3 scale, quat rot) {
    transform t = { 
        .key = id,
        .position = pos,
        .scale = scale,
        .rotation = rot,
    };
    hmputs(g_components.transforms, t);
}

void entity_delete_transform(entity_id id) {
    hmdel(g_components.transforms, id);
}

void entity_add_movement(entity_id id, vec3 velocity) {
    movement m = { id, velocity };
    hmputs(g_components.movements, m);
}

void entity_delete_movement(entity_id id) {
    hmdel(g_components.movements, id);
}

void entity_add_renderable(entity_id id, mesh* mesh, material* mat) {
    renderable r = { id, mesh, mat, true, true, true };
    hmputs(g_components.renderables, r);
}

void entity_delete_renderable(entity_id id) {
    hmdel(g_components.renderables, id);
}

void entity_add_camera(entity_id id, float fov, float znear, float zfar) {
    int w, h;
    window_size(&w, &h);
    camera cam = { 
        .key = id,
        .fov = fov,
        .znear = znear,
        .zfar = zfar, 
        .w = w, 
        .h = h,
    };
    hmputs(g_components.cameras, cam);
}

void entity_delete_camera(entity_id id) {
    hmdel(g_components.cameras, id);
}

void entity_add_player(entity_id id) {
    player p = { 0 };
    p.key = id;
    p.speed = 0.2f;
    hmputs(g_components.players, p);
}

/*  -------------- Systems. -------------- */

void entity_system_update() {
    for (size_t i = 0; i < hmlen(g_components.players); i++) {
        entity_player_control_system(&g_components.players[i]);
    }

    for (size_t i = 0; i < hmlen(g_components.cameras); i++) {
        entity_camera_system(&g_components.cameras[i]);
    }

    for (size_t i = 0; i < hmlen(g_components.renderables); i++) {
        entity_render_system(&g_components.renderables[i]);
    }
}

void entity_player_control_system(player* p) {
    transform* t = &hmgets(g_components.transforms, p->key);
    input* in = input_get();

    // Mouse.
    p->yaw   += in->mouse.dx * 0.001f;
    p->pitch += in->mouse.dy * 0.001f;

    quat q_pitch = quat_rotation_x(-p->pitch);
    quat q_yaw   = quat_rotation_y(-p->yaw);
    t->rotation  = quat_normalize(quat_mul_quat(q_pitch, q_yaw));

    if (p->pitch < -1.57f) p->pitch = -1.57f;
    else if (p->pitch > 1.57f) p->pitch = 1.57f;

    // Keyboard.
    if (pressed('W'))       p->forward = 1.0f;
    else if (pressed('S'))  p->forward = -1.0f;

    if (pressed('A'))       p->rightward = -1.0f;
    else if (pressed('D'))  p->rightward = 1.0f;

    if (pressed('Q'))       p->topward = -1.0f;
    else if (pressed('E'))  p->topward = 1.0f;

    if (pressed(VK_SHIFT))  p->speed = 2.0f;

    if (released('W') || released('S')) p->forward = 0.0f;
    if (released('A') || released('D')) p->rightward = 0.0f;
    if (released('Q') || released('E')) p->topward = 0.0f;
    if (released(VK_SHIFT)) p->speed = 0.2f;

    if (p->topward != 0 || p->rightward != 0 || p->forward != 0) {
        quat r_inv = quat_inverse(t->rotation);
        vec3 forward   = quat_mul_vec3(r_inv, vec3_new(0, 0, 1));
        vec3 rightward = quat_mul_vec3(r_inv, vec3_new(1, 0, 0));
        vec3 topward   = vec3_new(0, 1, 0);
        t->position = vec3_add(t->position, vec3_mul(forward, p->forward * p->speed));
        t->position = vec3_add(t->position, vec3_mul(rightward, p->rightward * p->speed));
        t->position = vec3_add(t->position, vec3_mul(topward, p->topward * p->speed));
    }
}

void entity_camera_system(camera* cam) {
    transform* t = &hmgets(g_components.transforms, cam->key);
    mat4 rotate = mat4_from_quat(t->rotation);
    mat4 translate = mat4_translate(t->position);
    
    cam->view  = mat4_inverse(mat4_mul(rotate, translate));
    cam->proj  = mat4_perspective(cam->fov, (float)cam->w / (float)cam->h, cam->znear, cam->zfar);
}

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
    // vec4 camera_position;
    // light m_lights[32];
    // int m_numLights = 0;
} constant;


void entity_render_system(renderable* r) {
    camera* cam = &g_components.cameras[0];

    transform* t = &hmgets(g_components.transforms, r->key);

    mat4 world = mat4_from_quat(quat_normalize(t->rotation));
    world = mat4_translate_mat4(world, t->position);

    constant cc;
    cc.world = world;
    cc.view = cam->view;
    cc.proj = cam->proj;
    material_set_data(r->mat, &cc, sizeof(constant));

    graphics_draw(r->mesh, r->mat);
}

transform* entity_get_transform(entity_id id) {
    return &hmgets(g_components.transforms, id);
}