#include "assets/material.h"

#include "vendor/stb_ds.h"

material* material_load(const char* vspath, const char* pspath) {
	material* mat = malloc(sizeof(material));
	memset(mat, 0, sizeof(material));

	// Load shaders.
	mat->vs = shader_new_load(vspath, "vsmain", SHADER_TYPE_VS);
	mat->ps = shader_new_load(pspath, "psmain", SHADER_TYPE_PS);

	return mat;
}

void material_unload(material* mat) {
	// Relase shaders.
	shader_delete(mat->ps);
	shader_delete(mat->vs);

	// Relase textures.
	for (size_t i = 0; i < mat->texs_size; i++) {
		d3d11_release_texture(mat->texs[i]);
	}
	if (mat->texs_size) {
		arrfree(*mat->texs);
	}

	mat->texs_size = 0;

	// Relase const buffer.
	d3d11_release_const_buffer(mat->cb);

	free(mat);
}

void material_add_texture(material* mat, texture* tex) {
	arrput(mat->texs, tex);
	mat->texs_size = arrlen(mat->texs);
}

void material_remove_texture(material* mat, uint index) {
	assert(index < arrlen(mat->texs));

	arrdel(mat->texs, index);
	mat->texs_size = arrlen(mat->texs);
}

void material_replace_texture(material* mat, uint index, texture* tex) {
	size_t len = arrlen(mat->texs);
	if (len == 0) {
		material_add_texture(mat, tex);
	} else if (index < len) {
		mat->texs[index] = tex;
	} else {
		warning("trying to replace non-existing texture");
	}
}

void material_set_data(material* mat, void* data, size_t size) {
	if (mat->cb == NULL) {
		mat->cb = d3d11_create_const_buffer(data, size);
	} else {
		d3d11_update_const_buffer(mat->cb, data);
	}
}

void material_set_mode(material* mat, cull_mode mode) {
	d3d11_set_rasterizer_state(mode == CULL_MODE_FRONT);
}
