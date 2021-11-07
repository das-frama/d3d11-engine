#ifndef MOTOR_GRAPHICS_H
#define MOTOR_GRAPHICS_H

/* Engine includes. */
#include "motor.h"
#include "platform/win32_platform.h"

/* Windows includes. */
#include <windows.h>

void graphics_init(int w, int h);
void graphics_close(void);

HWND graphics_get_window(void);
void graphics_get_window_size(int* w, int* h);

void graphics_window_update(void);

#endif // MOTOR_GRAPHICS_H
