#include <motor.h>

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
    float rot_angle;

    vec3 pos;
    planet_constant cc;
} planet;

static mesh* g_planet_mesh = NULL;
static mesh_data* g_planet_md = NULL;
static material* g_planet_mat = NULL;

planet* planet_new(float distance, float radius, float orb_period, float tilt_angle, float rot_speed, const char* texture_path) {
    planet* p = malloc(sizeof(planet));
    memset(p, 0, sizeof(planet));
    memset(&p->cc, 0, sizeof(planet_constant));

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
    p->radius     = radius;
    p->orb_period = orb_period;
    p->tilt_angle = tilt_angle;
    p->rot_speed  = rot_speed;
    p->rot_angle  = 0.0f;

    p->cc.light_position = vec4_new(0, 0, 0, 0);
    p->cc.light_radius   = 400.0f;

    return p;
}

void planet_delete(planet* p) {
	texture_unload(p->tex);
    free(p->rnd);
    free(p);
}

void planet_update(planet* p, float dt) {
    p->pos.x = p->distance * sin(p->orb_period*dt);
    p->pos.y = 0;
    p->pos.z = 0.8f * p->distance * cos(p->orb_period*dt);

    p->rot_angle += p->rot_speed * 0.05f;
}

void planet_draw(planet* p, const camera* cam) {
    mat4 scale = mat4_scale_by(p->radius);
    mat4 rotate = mat4_rotate_y(to_rad(p->rot_angle));
    mat4 translate = mat4_translate(p->pos);
    mat4 world = mat4_mul_x(3, scale, rotate, translate);

    p->cc.world = world;
    p->cc.view  = cam->view;
    p->cc.proj  = cam->proj;
    p->cc.camera_position = vec4_new_vec3(mat4_translation(cam->world));

    mat4 light_rot = mat4_rotate_y(-4.5f);
    p->cc.light_direction = vec4_new_vec3(mat4_z_direction(light_rot))  ;

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
	grid* grid = grid_new(1024, 1024, 64, 64, vec4_new(.5f, .5f, .5f, .25f));

	// Planet.
	planet* sun = planet_new(0, 20.0f, 2.0f, 0, 0, "assets\\textures\\sun.jpg");

	// Input.
	input in = {0};

	float planet_time = 0.0f;
	while (motor_running()) {
		frame_begin();

		// Platform events.
		platform_events();

		// Input events.
		input_update(&in);
		if (in.keys[27].pressed) {
			motor_stop_next();
		}

		// Update.
		camera_update(cam, &in, frame_dt());
		grid_update(grid, cam);
		planet_update(sun, planet_time);
		planet_time += 0.005f;

		// Render.
		graphics_clear_screen(0.0f, 0.0f, 0.0f, 0.0f);
		grid_draw(grid);
		planet_draw(sun, cam);
		graphics_present();

		frame_end();
	}

	// Release game resources.
	// mesh_unload(planet_mesh);
	// mesh_unload(skybox_mesh);
	planet_delete(sun);
	// light_delete(light);

	// Release engine resources.
	grid_delete(grid);
	camera_delete(cam);
	motor_close();

	return EXIT_SUCCESS;
}