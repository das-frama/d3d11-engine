#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/shader.h"

void graphics_init(int w, int h);
void graphics_close(void);

HWND graphics_get_window(void);
void graphics_get_window_size(int* w, int* h);

void graphics_clear_screen(float r, float g, float b, float a);
void graphics_draw_mesh(mesh* m, shader* s);
void graphics_present(void);

void graphics_window_update(void);

// void graphics_create_shaders(void);

#endif // MOTOR_GRAPHICS_H