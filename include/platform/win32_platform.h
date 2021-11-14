#ifndef MOTOR_WIN32_PLATFORM
#define MOTOR_WIN32_PLATFORM

#include "engine.h"

void win32_init(void);
void win32_close();

void win32_create(int w, int h);
void win32_show(bool show);
HWND win32_window();
void win32_size(int *w, int *h);
void win32_resize(int w, int h);
void win32_set_title(const char* title);

void win32_process_messages(void);
void win32_sleep(uint ms);

vec2 win32_mouse_pos(void);
uchar* win32_keyboard_state(void);

#endif // MOTOR_WIN32_PLATFORM