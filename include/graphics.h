#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

#include "engine.h"

#include "assets/renderable.h"\

typedef enum {
    FILL_MODE_SOLID,
    FILL_MODE_WIREFRAME,
} fill_mode;

void graphics_init(int w, int h);
void graphics_close(void);

void graphics_clear(float r, float g, float b, float a);
void graphics_draw(renderable* r);
void graphics_present(void);

#endif // MOTOR_GRAPHICS_H