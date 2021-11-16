#include <motor.h>

#include <stdio.h>

#define WIDTH  1920
#define HEIGHT 1080

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
    vec4 light_direction;
    vec4 cam_position;
} constant;

void light_update(constant* cc) {
	// static float rot_y;

	// mat4 temp = mat4_rotate_y(rot_y);
	// rot_y += 0.707f * dt;

	// cc->light_direction = vec4_new_vec3(mat4_z_direction(temp));
	cc->light_direction = vec4_new(0, 0, 1, 1);
}

void sphere_update(constant* cc, camera* cam, float dt) {
	// static float rot = 0.0f;
	// rot += 0.005f;
	
	// cc->world = mat4_translate(vec3_new(sin(0.707f * rot), 0, cos(0.707f * rot)));
	cc->world = mat4_id();
	cc->view  = cam->view;
	cc->proj  = cam->proj;
	cc->cam_position = vec4_new_vec3(mat4_translation(cam->world));
}

int main() {
	// init engine
	motor_init("assets", "Solar system", WIDTH, HEIGHT);

	// load assets
	mesh* sphere_mesh = mesh_load("assets\\meshes\\sphere.obj");

	texture* sphere_tex = texture_load("assets\\textures\\brick.png");
	material* sphere_mat = material_load(
		"assets\\shaders\\vertex_shader.hlsl", "assets\\shaders\\pixel_shader.hlsl"
	);
	material_add_texture(sphere_mat, sphere_tex);
	material_set_mode(sphere_mat, CULL_MODE_BACK);

	// Data.
	constant cc = {0};
	constant_grid cc_grid = {0};

	// Input.
	input in = {0};

	// Entities.
	camera cam = camera_new(WIDTH, HEIGHT);
	grid g = generate_grid(1024, 1024, 64, 64, vec4_new(.5f, .5f, .5f, 1));

	while(motor_running()) {
		frame_begin();

		// Platform events.
		platform_events();

		// Input events.
		input_update(&in);

		// Update.
		camera_update(&cam, &in, frame_dt());
		light_update(&cc);

		grid_update(&g, &cam);

		sphere_update(&cc, &cam, frame_dt());
		material_set_data(sphere_mat, &cc, sizeof(constant));

		// Render.
        graphics_clear_screen(0.0f, 0.0f, 0.0f, 1.0f);

        // graphics_draw(&g.m, &g.mat);
        graphics_draw_grid(&g);
        graphics_draw(sphere_mesh, sphere_mat);

        graphics_present();

        // print_cam(&cam);

		frame_end();
	}

	// unload assets
	// material_unload(sphere_mat);
	// mesh_unload(sphere_mesh);

	// release engine resources
	motor_close();

	return 0;
}