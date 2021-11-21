#ifndef MOTOR_WINDOW_H
#define MOTOR_WINDOW_H

#include "engine.h"

void window_init(void);
void window_close(void);

void window_create(int w, int h);
void window_show(bool show);
void window_size(int* w, int* h);
void window_resize(int w, int h);
void window_set_title(const char* title);

void window_show_mouse(bool show);
void window_set_mouse_pos(float x, float y);

void window_events(void);

#endif // MOTOR_WINDOW_H