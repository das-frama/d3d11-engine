#ifndef MOTOR_SHADER_H
#define MOTOR_SHADER_H

#include "engine.h"

typedef enum {
	SHADER_TYPE_VS,
	SHADER_TYPE_PS,
} shader_type;

typedef struct { 
	void* ptr;
	shader_type type;
} shader;

shader* shader_load(const char* filename, const char* entry_point, shader_type type);
void shader_unload(shader* s);

#endif // MOTOR_SHADER_H