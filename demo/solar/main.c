#include <motor.h>
#include <vendor/stb_ds.h>

static mesh* g_mesh = NULL;
static material* g_mat = NULL;

typedef struct {
    entity_id key;
    float distance;
    float orb_period;
    vec2 pos;
} planet_component;

typedef struct {
    planet_component* planets;
} user_component_list;

static user_component_list g_components = {0};

void create_sphere() {
    entity_id id = entity_create();

    planet_component pc = {0};
    pc.key = id;
    pc.distance = rand() % 100;
    pc.orb_period = rand() % 5 + 1;
    hmputs(g_components.planets, pc);
    // Generate and set sphere mesh.
    if (g_mesh == NULL) {
        mesh_data* md = generate_sphere(10.0f, 30, 30);
        g_mesh = mesh_new_data(md);
        mesh_data_delete(md);
    }

    // // Set default material.
    if (g_mat == NULL) {
        g_mat = material_new();
        g_mat->vs = shader_new_load("assets\\shaders\\planet_vs.hlsl", "vsmain", SHADER_TYPE_VS);
        g_mat->ps = shader_new_load("assets\\shaders\\planet_ps.hlsl", "psmain", SHADER_TYPE_PS);
        g_mat->mode = CULL_MODE_FRONT;
    }

    entity_add_renderable(id, g_mesh, g_mat);    
    entity_add_transform(id, vec3_new(0,0,0), vec3_new(1,1,1), quat_id());
}

void spheres_rotation_system(float dt) {
    for (int i = 0; i < hmlen(g_components.planets); i++) {
        planet_component* pc = &g_components.planets[i];
        transform* t = entity_get_transform(pc->key);
        t->position.x = pc->distance * sinf(pc->orb_period*dt);
        t->position.y = 0;
        t->position.z = 0.8f * pc->distance * cosf(pc->orb_period*dt);

        // p->rot_angle += p->rot_speed;
    }
}

int main() {
    motor_init("My Game", 1920, 1080);

    // Entities.
    entity_id player = entity_create();
    entity_add_player(player);
    entity_add_camera(player, 1.25f, 0.1f, 10000.0f);
    entity_add_transform(player, vec3_new(0,0,0), vec3_new(1,1,1), quat_id());

    entity_id skybox = skybox_new();
    entity_id grid = grid_new(1024, 1024, 64, 64);

    g_components.planets = NULL;

    // Main game loop.
    while (!game_should_quit()) {
        frame_begin();

        // Update window events.
        window_events();
        
        // Update input.
        input_update();
        if (input_key_released('G')) {
            game_switch_playmode();
        }
        if (input_key_pressed(27)) {
            game_should_stop_next();
        }
        if (input_key_released(32)) {
            create_sphere();
        }
        if (input_key_released('R')) {
            graphics_switch_fill_mode();
        }

        // Draw.
        graphics_clear(0.0f, 0.0f, 0.0f, 1.0f);
        spheres_rotation_system(frame_total_time());
        entity_system_update(); 
        graphics_present();
        frame_end();
    }

    skybox_delete(skybox);
    grid_delete(grid);
    motor_close();

    return EXIT_SUCCESS;
}