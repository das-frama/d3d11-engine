#include "assets/material.h"

#include "vendor/stb_ds.h"

material* material_new() {
	material* mat = malloc(sizeof(material));
	memset(mat, 0, sizeof(material));

	return mat;
}

material* material_new_(shader* vs, shader* ps, texture* tex, void* data, size_t data_size) {
	material* mat = material_new();

	// Set shaders.
	mat->vs = vs;
	mat->ps = ps;

	// Set texture.
	if (tex) {
		material_add_texture(mat, tex);
	}

	// Set data.
	if (data) {
		material_set_data(mat, data, data_size);
	}

	// Set raster state mode.
	mat->mode = CULL_MODE_BACK;

	return mat;
}

void material_delete(material* mat) {
	// Relase shaders.
	shader_delete(mat->vs);
	shader_delete(mat->ps);
	mat->vs = NULL;
	mat->ps = NULL;

	// Relase textures.
	for (size_t i = 0; i < mat->texs_size; i++) {
		texture_delete(mat->texs[i]);
	}
	if (mat->texs_size) {
		arrfree(*mat->texs);
	}
	mat->texs = NULL;
	mat->texs_size = 0;

	// Relase const buffer.
	if (mat->cb) {
		d3d11_release_constant_buffer(mat->cb);
		mat->cb = NULL;
	}

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
		mat->cb = d3d11_create_constant_buffer(data, size);
	} else {
		d3d11_update_constant_buffer(mat->cb, data);
	}
}