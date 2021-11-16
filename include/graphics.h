#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

#include "engine.h"

#include "assets/renderable.h"

void graphics_init(int w, int h);
void graphics_close(void);

void graphics_window_size(int* w, int* h);
void graphics_window_resize(int w, int h);
void graphics_window_set_title(const char* title);

void graphics_do_one_frame(void);

void graphics_clear(float r, float g, float b, float a);
void graphics_draw(renderable* r);
void graphics_present(void);


#endif // MOTOR_GRAPHICS_H