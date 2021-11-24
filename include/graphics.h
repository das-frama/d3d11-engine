#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/material.h"

typedef enum {
    FILL_MODE_SOLID,
    FILL_MODE_WIREFRAME,
} fill_mode;

void graphics_init(int w, int h);
void graphics_close(void);

void graphics_clear(float r, float g, float b, float a);
void graphics_draw(mesh* mesh, material* mat);
void graphics_present(void);

#endif // MOTOR_GRAPHICS_H