#include "assets/shader.h"

extern renderer* rnd;

shader* shader_new() {
	shader* s = malloc(sizeof(shader));
	memset(s, 0, sizeof(shader));
	return s;
}

void shader_delete(shader* s) {
	d3d11_release_vertex_shader(&s->vs);
	d3d11_release_pixel_shader(&s->ps);
	free(s);
}

void shader_load_fx(shader* s, const char* fx_path) {
	void* shader_byte_code = NULL;
	size_t byte_code_size = 0;


	d3d11_compile_vertex_shader(fx_path, "vsmain", &shader_byte_code, &byte_code_size);
	s->vs = d3d11_create_vertex_shader(rnd, shader_byte_code, byte_code_size);
	d3d11_release_compiled_shaders();

	d3d11_compile_pixel_shader(fx_path, "psmain", &shader_byte_code, &byte_code_size);
	s->ps = d3d11_create_pixel_shader(rnd, shader_byte_code, byte_code_size);
	d3d11_release_compiled_shaders();

	// s->vs = malloc(sizeof(vertex_shader));
	// memcpy(a->vs, &vs, sizeof(vertex_shader));

	// s->ps = malloc(sizeof(pixel_shader));
	// memcpy(a->ps, &ps, sizeof(pixel_shader));
}