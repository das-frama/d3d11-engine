#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/material.h"

typedef enum {
    FILL_MODE_SOLID,
    FILL_MODE_WIREFRAME,
} fill_mode;

void graphics_init();
void graphics_close(void);

void graphics_clear(float r, float g, float b, float a);
void graphics_draw(mesh* mesh, material* mat);
void graphics_present(void);

void graphics_switch_fill_mode();

#endif // MOTOR_GRAPHICS_H