#include <motor.h>

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
} constant;

mat4 world_cam = {0};
mat4 view_cam  = {0};
mat4 proj_cam  = {0};

int w = 1440, h = 900;

void update_cam() {	
	world_cam = mat4_id();

	vec3 wct = mat4_translation(world_cam);
	vec3 wcz = mat4_z_direction(world_cam);
	vec3 wcx = mat4_x_direction(world_cam);
	vec3 new_pos = vec3_add(3, wct, wcz, wcx);

    mat4_translate(&world_cam, new_pos);
    mat4_inverse(&world_cam);

    view_cam = world_cam;
	mat4_perspective_fov_lh(&proj_cam, 1.25f, (float)w / (float)h, 0.1f, 1000.0f);
}

int main() {
	world_cam = mat4_new();
	view_cam  = mat4_new();
	proj_cam  = mat4_new();

	motor_init("assets", w, h);

	// load assets
	asset* sphere_obj	  = asset_load("spaceship.obj");
	shader* sphere_shader = shader_new();
	shader_load_hlsl(sphere_shader, "assets\\vertex_shader.hlsl", "vsmain");
	shader_load_hlsl(sphere_shader, "assets\\pixel_shader.hlsl", "psmain");

	mat4_translate(&world_cam, (vec3) {0, 0, -50});
	
	constant_buffer cb = graphics_create_constant_buffer(sizeof(constant));

	while(motor_running()) {
		frame_begin();

        // update_cam();
        // update cc;
        {
            constant cc = { 0 };
        	cc.world = mat4_id();
        	cc.view = mat4_id();
        	cc.proj = mat4_id();
        	mat4_ortho_lh(&cc.proj, (float)w / 100.f, (float)h / 100.0f, -4.0f, 4.0f);

    		graphics_update_constant_buffer(&cb, &cc);
    	}


      //   	vec3 new_pos = mat4_translation(world_cam);
      //   	new_pos = vec3_add(2, new_pos, mat4_z_direction(world_cam));
      //   	cc.world = mat4_id();
      //   	// mat4_scale(&cc.world, {1.0f, 1.0f, 1.0f})
      //   	mat4_translate(&cc.world, new_pos);
      //   	cc.view = view_cam;
    		// cc.proj = proj_cam;

        graphics_clear_screen(0.1f, 0.1f, 0.1f, 1.0f);
        graphics_draw_mesh(sphere_obj->ptr, sphere_shader, &cb);
        graphics_present();
        
        motor_process_events();

        frame_end();
	}

	// unload assets
	shader_delete(sphere_shader);
	asset_unload(sphere_obj);

	// release engine resources
	motor_close();

	return 0;
}