#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/material.h"

void graphics_init(int w, int h);
void graphics_close(void);

void graphics_window_size(int* w, int* h);
void graphics_window_resize(int w, int h);
void graphics_window_set_title(const char* title);

void graphics_do_one_frame(void);

void graphics_clear(float r, float g, float b, float a);
void graphics_draw(mesh* m, material* mat);
void graphics_present(void);

void graphics_draw_text(int x, int y, const char* text);
// const_buffer* graphics_create_const_buffer(size_t buffer_size);
// void graphics_update_const_buffer(const const_buffer* cb, void* buffer);

// void graphics_create_shaders(void);

#endif // MOTOR_GRAPHICS_H