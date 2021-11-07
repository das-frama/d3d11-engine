#ifndef MOTOR_WIN32_PLATFORM
#define MOTOR_WIN32_PLATFORM

/* Engine includes. */
#include "utils/utils.h"
#include "utils/log.h"

/* Windows includes. */
#include <windows.h>

bool win32_init(void);
void win32_close(HWND hwnd);

HWND win32_create_window(int w, int h);
void win32_show_window(HWND hwnd, bool show);
void win32_resize_window(HWND hwnd, int w, int h);
void win32_get_window_size(HWND hwnd, int *w, int *h);

void win32_broadcast(void);

#endif // MOTOR_WIN32_PLATFORM
