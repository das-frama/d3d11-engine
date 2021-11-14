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
	mesh* sphere_mesh = mesh_load("assets\\meshes\\house.obj");
	material* sphere_mat = material_load(
		"assets\\shaders\\vertex_shader.hlsl", "assets\\shaders\\pixel_shader.hlsl"
	);
	material_set_data(sphere_mat, &cc, sizeof(constant));
	
	texture* sphere_tex = texture_load("assets\\textures\\brick.png");
	material_add_texture(sphere_mat, sphere_tex);
	material_set_mode(sphere_mat, CULL_MODE_BACK);

	// create entities
	input in = {0};
	float rot_y = 0.0f;
	float top = 0.0f;
	float right = 0.0f;

	while(motor_running()) {
		frame_begin();

		// Platform events.
		platform_events();

		// Input events.
		{
			input_update(&in);
			if (in.mouse.left_pressed) {
				rot_y += frame_dt() * 0.1f;
			} else {
				rot_y -= frame_dt() * 0.1f;
				if (rot_y <= 0) {
					rot_y = 0.0f;
				}
			}
			if (in.keys['W'].pressed) {
				top += 1.0f;
			} else if (in.keys['S'].pressed) {
				top -= 1.0f;
			}

			if (in.keys['A'].pressed) {
				right -= 1.0f;
			} else if (in.keys['D'].pressed) {
				right += 1.0f;
			}
		}

		// Update.
		{
			mat4 temp = mat4_id();
			mat4_rotate_y(&temp, rot_y);
			cc.world = mat4_mul(cc.world, temp);

			temp = mat4_id();
		    vec3 new_pos = vec3_mul(mat4_x_direction(temp), right * 0.1f);
		    mat4_translate(&cc.world, new_pos);

			temp = mat4_id();
		    new_pos = vec3_add(2, new_pos, vec3_mul(mat4_y_direction(temp), top * 0.1f));
		    mat4_translate(&cc.world, new_pos);

			material_set_data(sphere_mat, &cc, sizeof(constant));
		}

		// Render.
		{
	        graphics_clear_screen(0.0f, 0.0f, 0.0f, 1.0f);
	        graphics_draw(sphere_mesh, sphere_mat);
	        graphics_present();
		}

		frame_end();
	}

	// unload assets
	// material_unload(sphere_mat);
	// mesh_unload(sphere_mesh);

	// delete resources.
	// camera_delete(c);

	// release engine resources
	motor_close();

	return 0;
}