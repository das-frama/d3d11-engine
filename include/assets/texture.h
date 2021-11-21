#ifndef MOTOR_TEXTURE_H
#define MOTOR_TEXTURE_H

#include "engine.h"

typedef struct {
	void* ptr;
	int w, h, bit;
} texture;

texture* texture_new(void);
texture* texture_new_load(const char* filename);
void texture_delete(texture* tex);

#endif // MOTOR_TEXTURE_H