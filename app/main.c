#include <motor.h>

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
} constant;

int w = 1440, h = 900;
constant cc = { 0 };

void update_cc() {
	cc.world = mat4_id();
	cc.view = mat4_id();
	cc.proj = mat4_id();
	mat4_ortho_lh(&cc.proj, (float)w / 100.f, (float)h / 100.0f, -10.0f, 10.0f);
}

int main() {
	// init engine
	motor_init("assets", "Solar system", w, h);

	update_cc();

	// load assets
	mesh* sphere_mesh = mesh_load("assets\\meshes\\spaceship.obj");
	material* sphere_mat = material_load(
		"assets\\shaders\\vertex_shader.hlsl", "assets\\shaders\\pixel_shader.hlsl"
	);
	material_set_data(sphere_mat, &cc, sizeof(constant));
	
	// texture* sphere_tex = texture_load("assets\\textures\\brick.png");
	// material_add_texture(sphere_mat, sphere_tex);
	// material_set_mode(sphere_mat, CULL_MODE_BACK);

	while(motor_running()) {
		frame_begin();

		// Platform events.
		process_events();

		// Input events.

		// Update.

		// Render.
        graphics_clear_screen(0.0f, 0.0f, 0.0f, 1.0f);
        graphics_draw(sphere_mesh, sphere_mat);
        graphics_present();

		frame_end();
	}

	// unload assets
	// material_unload(sphere_mat);
	// mesh_unload(sphere_mesh);

	// release engine resources
	motor_close();

	return 0;
}