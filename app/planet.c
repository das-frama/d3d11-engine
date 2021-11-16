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
    texture* tex;

    float distance;
    float radius;
    float orb_period;
    float rot_speed;
    float tilt_angle;

    vec3 pos;
    planet_constant cc;
} planet;

static renderable* planet_r = NULL;

// Create a new planet.
planet* planet_new(
   float distance, float radius, float orb_period,
   float tilt_angle, float rot_speed, const char* texture_path
) {
    planet* p = malloc(sizeof(planet));
    memset(p, 0, sizeof(planet));
    memset(&p->cc, 0, sizeof(planet_constant));

    // load assets
    if (planet_r == NULL) {
        mesh* planet_mesh     = mesh_load("assets\\meshes\\sphere.obj");
        material* planet_mat  = material_load("assets\\shaders\\vertex_shader.hlsl", 
                                           "assets\\shaders\\pixel_shader.hlsl");
        planet_r = renderable_new();
        renderable_add_mesh(planet_r, planet_mesh);
        renderable_add_material(planet_r, planet_mat);
    }
    p->object = planet_r;
    p->tex = texture_load(texture_path);

    p->distance   = distance;
    p->radius     = radius;
    p->orb_period = orb_period;
    p->tilt_angle = tilt_angle;
    p->rot_speed  = rot_speed;

    return p;
}

void planet_delete(planet *p) {
    texture_unload(p->tex);
    p->object = NULL;
    p->tex = NULL;
    free(p);
}
void planet_delete_static() {
    renderable_delete(planet_r);
}

void planet_update(planet* p, float dt) {
    p->pos.x = p->distance * sin(p->orb_period*dt);
    p->pos.y = 0;
    p->pos.z = 0.8f * p->distance * cos(p->orb_period*dt);
}

void planet_draw(planet* p, const camera* cam) {
    mat4 scale = mat4_scale_by(p->radius);
    mat4 translate = mat4_translate(p->pos);
    
    p->cc.world = mat4_mul(scale, translate);
    p->cc.view  = cam->view;
    p->cc.proj  = cam->proj;
    p->cc.cam_position = vec4_new_vec3(mat4_translation(cam->world));

    material_replace_texture(p->object->material, 0, p->tex);
    material_set_data(p->object->material, &p->cc, sizeof(planet_constant));

    graphics_draw(p->object);
}