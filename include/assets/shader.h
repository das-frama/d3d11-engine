#ifndef MOTOR_SHADER_H
#define MOTOR_SHADER_H

#include "renderer/d3d11_renderer.h"

typedef struct {
	vertex_shader vs;
	pixel_shader  ps;
} shader;

shader* shader_new(void);
void shader_delete(shader* s);
void shader_load_fx(shader* s, const char* fx_path);

#endif // MOTOR_SHADER_H