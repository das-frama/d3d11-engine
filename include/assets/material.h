#ifndef MOTOR_MATERIAL_H
#define MOTOR_MATERIAL_H

#include "engine.h"

#include "assets/shader.h"
#include "assets/texture.h"

#include "renderer/d3d11_renderer.h"

typedef enum {
	CULL_MODE_BACK,
	CULL_MODE_FRONT,
} cull_mode;

typedef struct {
	shader* vs;
	shader* ps;
	const_buffer* cb;
	cull_mode cull_mode;

	texture** texs; // array
	size_t texs_size;
} material;

// material* material_new(void);
material* material_load(const char* vspath, const char* pspath);
void material_unload(material* m);

void material_add_texture(material* m, texture* t);
void material_remove_texture(material* m, uint index);
void material_replace_texture(material* m, uint index, texture* t);

void material_set_data(material* m, void* data, size_t size);
void material_set_mode(material* m, cull_mode mode);

#endif // MOTOR_MATERIAL_H