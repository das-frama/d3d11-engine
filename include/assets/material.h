#ifndef MOTOR_MATERIAL_H
#define MOTOR_MATERIAL_H

#include "engine.h"

#include "assets/shader.h"
#include "assets/texture.h"
#include "renderer/d3d11_renderer.h"

typedef enum {
	CULL_MODE_FRONT,
	CULL_MODE_BACK,
	CULL_MODE_WIREFRAME,
} cull_mode;

typedef struct {
	shader* vs;
	shader* ps;
	d3d11_buffer* cb;
	texture** texs; // array
	size_t texs_size;
	cull_mode mode;
} material;

material* material_new(void);
material* material_new_(shader* vs, shader* ps, texture* tex, void* data, size_t data_size);
void material_delete(material* mat);

void material_add_texture(material* mat, texture* tex);
void material_remove_texture(material* mat, uint index);
void material_replace_texture(material* mat, uint index, texture* tex);

void material_set_data(material* mat, void* data, size_t size);

#endif // MOTOR_MATERIAL_H