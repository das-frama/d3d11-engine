#ifndef MOTOR_GEOMETRY_H
#define MOTOR_GEOMETRY_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/material.h"

#include "entities/camera.h"

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
} constant_grid;

typedef struct {
	mesh m;
	material mat;
	constant_grid cc;
} grid;


grid generate_grid(float width, float depth, int m, int n, vec4 color);
void grid_update(grid* g, const camera* cam);

#endif // MOTOR_GEOMETRY_H