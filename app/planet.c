#include <motor.h>
#include <vendor/stb_ds.h>

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
    vec4 light_direction;
    vec4 camera_position;
    vec4 light_position;
    float light_radius;
} planet_constant;

typedef struct {
    renderable* rnd;
    texture* tex;

    float distance;
    float radius;
    float orb_period;
    float rot_speed;
    float tilt_angle;

    vec3 pos;
    planet_constant cc;
} planet;

static mesh* planet_mesh = NULL;
static material* planet_mat = NULL;

// Create a new planet.
planet* planet_new(
   float distance, float radius, float orb_period,
   float tilt_angle, float rot_speed, const char* texture_path
) {
    planet* p = malloc(sizeof(planet));
    memset(p, 0, sizeof(planet));
    memset(&p->cc, 0, sizeof(planet_constant));

    // load assets
    if (planet_mesh == NULL) {
        planet_mesh = mesh_load("assets\\meshes\\sphere_hq.obj");
    }
    if (planet_mat == NULL) {
       planet_mat = material_load("assets\\shaders\\planet_vs.hlsl", "assets\\shaders\\planet_ps.hlsl");
    }
    
    p->rnd = renderable_new();
    renderable_add_mesh(p->rnd, planet_mesh);
    renderable_set_material(p->rnd, planet_mat);
    p->tex = texture_load(texture_path);

    // set members
    p->distance   = distance;
    p->radius     = radius;
    p->orb_period = orb_period;
    p->tilt_angle = tilt_angle;
    p->rot_speed  = rot_speed;

    p->cc.light_position = vec4_new(0, 0, 0, 0);
    p->cc.light_radius   = 400.0f;

    return p;
}

planet* sun_new() {
    planet* sun = planet_new(0.0f, 20.0f, 0.0f, 0.0f, 0.0f, "assets\\textures\\sun.jpg");

    material* sun_mat = material_load("assets\\shaders\\planet_vs.hlsl", "assets\\shaders\\sun_ps.hlsl");
    renderable_set_material(sun->rnd, sun_mat);

    return sun;
}

void planet_delete_static() {
    mesh_unload(planet_mesh);
    material_unload(planet_mat);
}

void planet_delete(planet *p) {
    texture_unload(p->tex);
    free(p->rnd);
    free(p);
}

void sun_delete(planet* sun) {
    material_unload(sun->rnd->material);
    planet_delete(sun);
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
    p->cc.camera_position = vec4_new_vec3(mat4_translation(cam->world));

    // light
    mat4 light_rot = mat4_rotate_y(-4.5f);
    p->cc.light_direction = vec4_new_vec3(mat4_z_direction(light_rot))  ;

    material_set_data(p->rnd->material, &p->cc, sizeof(planet_constant));
    material_replace_texture(p->rnd->material, 0, p->tex);

    graphics_draw(p->rnd);
}