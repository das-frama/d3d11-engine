#include <motor.h>

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
    vec4 light_direction;
    vec4 cam_position;
} planet_constant;

typedef struct {
    renderable* object;

    float distance;
    float scale;
    float orb_period;
    float rot_speed;
    float rot_angle;

    planet_constant cc;
} planet;

// Create a new planet.
planet* planet_new(float distance, float scale) {
    planet* p = malloc(sizeof(planet));
    memset(p, 0, sizeof(planet));
    memset(&p->cc, 0, sizeof(planet_constant));

    // load assets
    mesh* sphere_mesh = mesh_load("assets\\meshes\\sphere.obj");
    material* sphere_mat = material_load("assets\\shaders\\vertex_shader.hlsl", "assets\\shaders\\pixel_shader.hlsl");
    material_add_texture(sphere_mat, texture_load("assets\\textures\\brick.png"));

    p->object = renderable_new();
    renderable_add_mesh(p->object, sphere_mesh);
    renderable_add_material(p->object, sphere_mat);

    p->distance = distance;
    p->scale    = scale;

    return p;
}

void planet_delete(planet *p) {
    renderable_delete(p->object);
    free(p);
}

void planet_update(planet* p, const camera* cam, float dt) {
    p->cc.world = mat4_id();
    p->cc.view  = cam->view;
    p->cc.proj  = cam->proj;
    p->cc.cam_position = vec4_new_vec3(mat4_translation(cam->world));
    material_set_data(p->object->material, &p->cc, sizeof(planet_constant));
}

void planet_draw(planet* p) {
    graphics_draw(p->object);
}