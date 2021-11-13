#ifndef MOTOR_TEXTURE_H
#define MOTOR_TEXTURE_H

#include "engine.h"

typedef struct {
	ID3D11Texture2D* ptr;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* smp;
} texture;

texture* texture_load(const char* filename);
void texture_unload(texture* t);

#endif // MOTOR_TEXTURE_H