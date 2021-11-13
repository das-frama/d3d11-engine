#ifndef MOTOR_TEXTURE_H
#define MOTOR_TEXTURE_H

#include "engine.h"

typedef struct {
	void* ptr;
	ID3D11ShaderResourceView* srv;
	void* smp;
} texture;

texture* texture_load(const char* filename);
void texture_unload(texture* t);

#endif // MOTOR_TEXTURE_H