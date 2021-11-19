#include "assets/shader.h"

#include "renderer/d3d11_renderer.h"

shader* shader_new(void) {
	shader* s = malloc(sizeof(shader));
	memset(s, 0, sizeof(shader));

	return s;
}

shader* shader_new_load(const char* filename, const char* entry_point, shader_type type) {
	// if provided file doesn't have .hlsl extension...
	if (!file_ext_eq(filename, "hlsl") && !file_ext_eq(filename, "fx")) {
		error("shaders allows load either .hlsl or .fx files");
	}

	shader* s = shader_new();
	s->type = type;

	void* byte_code = NULL;
	size_t byte_code_size = 0;
	if (s->type == SHADER_TYPE_PS) {
		d3d11_compile_pixel_shader(filename, entry_point, &byte_code, &byte_code_size);
		s->ptr = d3d11_create_pixel_shader(byte_code, byte_code_size);
	} else if (s->type == SHADER_TYPE_VS) {
		d3d11_compile_vertex_shader(filename, entry_point, &byte_code, &byte_code_size);
		s->ptr = d3d11_create_vertex_shader(byte_code, byte_code_size);
	}
	d3d11_release_compiled_shaders();

	return s;
}

void shader_delete(shader* s) {
	if (s->type == SHADER_TYPE_VS) {
		d3d11_release_vertex_shader(s);
	} else if (s->type == SHADER_TYPE_PS) {
		d3d11_release_pixel_shader(s);
	}

	free(s);
}

void shader_set_byte(shader* s, const void* byte_code, size_t byte_code_size) {
	if (s->type == SHADER_TYPE_PS) {
		s->ptr = d3d11_create_pixel_shader(byte_code, byte_code_size);
	} else if (s->type == SHADER_TYPE_VS) {
		s->ptr = d3d11_create_vertex_shader(byte_code, byte_code_size);
	}
}