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

shader* shader_new(void);
shader* shader_new_load(const char* filename, const char* entry_point, shader_type type);
void shader_delete(shader* s);
void shader_set_byte(shader* s, const void* byte_code, size_t byte_code_size);

#endif // MOTOR_SHADER_H