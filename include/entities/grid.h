#ifndef MOTOR_GRID_H
#define MOTOR_GRID_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/material.h"

#include "entities/camera.h"

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
    vec4 color;
} constant_grid;

typedef struct {
    mesh* mesh;
    material* material;

    constant_grid cc;
} grid;

grid* grid_new(float width, float depth, int m, int n, vec4 color);
void grid_delete(grid* g);

void grid_update(grid* g, const camera* cam);
void grid_draw(const grid* g);

#endif // MOTOR_GRID_H