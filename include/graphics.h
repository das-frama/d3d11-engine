#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/shader.h"

#include "renderer/d3d11_renderer.h"

void graphics_init(int w, int h);
void graphics_close(void);

HWND graphics_get_window(void);
void graphics_get_window_size(int* w, int* h);

void graphics_clear_screen(float r, float g, float b, float a);
void graphics_draw_mesh(mesh* m, shader* s, constant_buffer* cb);
void graphics_present(void);

void graphics_window_update(void);

constant_buffer graphics_create_constant_buffer(size_t buffer_size);
void graphics_update_constant_buffer(constant_buffer* cb, void* buffer);

// void graphics_create_shaders(void);

#endif // MOTOR_GRAPHICS_H