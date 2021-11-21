#include <motor.h>

#define PLANET_COUNT 5

typedef struct {
    renderable* rnd;
    texture* tex;

    float distance;
    float diameter;
    float orb_period;
    float rot_speed;
    float tilt_angle;

    float rot_angle;
    vec3 pos;
} planet;

static mesh* g_planet_mesh = NULL;
static mesh_data* g_planet_md = NULL;
static material* g_planet_mat = NULL;

planet* planet_new(float distance, float diameter, float orb_period, float tilt_angle, 
                   float rot_speed, const char* texture_path) {
    planet* p = calloc(1, sizeof(planet));

	if (g_planet_md == NULL) {
		g_planet_md = generate_sphere(1.0f, 30, 30);
	}
	if (g_planet_mesh == NULL) {
		g_planet_mesh = mesh_new();
		mesh_data_set(g_planet_mesh, g_planet_md);
        mesh_data_delete(g_planet_md);
	}
	if (g_planet_mat == NULL) {
       g_planet_mat = material_load("assets\\shaders\\planet_vs.hlsl", "assets\\shaders\\planet_ps.hlsl");
    }

    p->rnd = renderable_new();
    renderable_add_mesh(p->rnd, g_planet_mesh);
    renderable_set_material(p->rnd, g_planet_mat);
    p->tex = texture_load(texture_path);

    // set members
    p->distance   = distance;
    p->diameter   = diameter;
    p->orb_period = orb_period;
    p->tilt_angle = tilt_angle;
    p->rot_speed  = rot_speed;
    p->rot_angle  = 0.0f;

    return p;
}

planet* sun_new() {
    planet* sun = planet_new(0, 20, 0, 0, 0, "assets\\textures\\sun.jpg");
    material* sun_material = material_load("assets\\shaders\\planet_vs.hlsl", "assets\\shaders\\sun_ps.hlsl");
    renderable_set_material(sun->rnd, sun_material);

    return sun;
}


void planet_delete(planet* p) {
    texture_unload(p->tex);
    renderable_delete(p->rnd);
    free(p);
}

void sun_delete(planet* sun) {
    material_unload(sun->rnd->material);
    planet_delete(sun);
}


void planet_delete_static() {
    mesh_delete(g_planet_mesh);
    material_unload(g_planet_mat);
}

void planet_update(planet* p, float dt) {
    p->pos.x = p->distance * sin(p->orb_period*dt);
    p->pos.y = 0;
    p->pos.z = 0.8f * p->distance * cos(p->orb_period*dt);

    p->rot_angle += p->rot_speed;
}

void planet_draw(planet* p, const camera* cam) {
    mat4 scale = mat4_scale_by(p->diameter);
    mat4 rotate = mat4_rotate_y(to_rad(p->rot_angle));
    mat4 translate = mat4_translate(p->pos);
    mat4 world = mat4_mul_x(3, scale, rotate, translate);

    p->cc.world = world;
    p->cc.view  = cam->view;
    p->cc.proj  = cam->proj;
    p->cc.camera_position = vec4_new_vec3(mat4_translation(cam->world));

    material_set_data(p->rnd->material, &p->cc, sizeof(planet_constant));
    material_replace_texture(p->rnd->material, 0, p->tex);

    graphics_draw(p->rnd);
}

int main() {
	// init engine.
	motor_init("D3D11 Solar System", 1920, 1080);

	// Camera.
	camera* cam = camera_new(0, 0, 50);

	// Grid.
	grid* grid = grid_new(1024, 1024, 64, 64, color(.5f, .5f, .5f, .25f));

    // Skybox.
    skybox* sky = skybox_new("assets\\textures\\milkyway.jpg");

	// Planets.
    planet* planets[PLANET_COUNT];
    // sun
	planets[0] = sun_new();
    // mercury
    planets[1] = planet_new(30, 1, 1, 0.0f, 0.0171f, "assets\\textures\\mercury.jpg");
    // venus
    planets[2] = planet_new(42, 2.85f, 0.39f, -177.4f, 0.0041f, "assets\\textures\\venus.jpg");
    // earth
    planets[3] = planet_new(55, 3, 0.24f, -23.0f, 1, "assets\\textures\\earth_day.jpg");
    // mars
    planets[4] = planet_new(68, 1.5f, 0.13f, -25.2f, 0.9756f, "assets\\textures\\mars.jpg");

	// Input.
	input in = {0};

	float planet_time = 0.0f;
	while (motor_running()) {
		frame_begin();

		// Platform events.
		window_events();

		// Input events.
		input_update(&in);
		if (in.keys[27].pressed) {
			motor_stop_next();
		}

		// Update.
		camera_update(cam, &in, frame_dt());
		grid_update(grid, cam);
        for (int i = 0; i < PLANET_COUNT; i++) {
		  planet_update(planets[i], planet_time);
        }
		planet_time += 0.005f;

		// Render.
		graphics_clear_screen(0.0f, 0.0f, 0.0f, 0.0f);
		grid_draw(grid);
        graphics_draw(sky_rnd);
		for (int i = 0; i < PLANET_COUNT; i++) {
          planet_draw(planets[i], cam);
        }
		graphics_present();

		frame_end();
	}

	// Release game resources.
    sun_delete(planets[0]);
	for (int i = 1; i < PLANET_COUNT; i++) {
        planet_delete(planets[i]);
    }
    renderable_delete(sky_rnd);

	// Release engine resources.
	grid_delete(grid);
	camera_delete(cam);
    skybox_delete(sky);
	motor_close();

	return EXIT_SUCCESS;
}