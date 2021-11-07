#include "platform/win32_platform.h"

extern bool running;

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_CLOSE:
        case WM_DESTROY:
            running = false;
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

bool win32_init() {
	WNDCLASSA wc = {0};
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.hInstance = NULL;
    wc.lpszClassName = "motor_engine";
    wc.lpszMenuName = "";
    wc.style = 0;
    wc.lpfnWndProc = &wnd_proc;
    if (!RegisterClassA(&wc)) {
        return false;
    }

    return true;
}

void win32_close(HWND hwnd) {
    DestroyWindow(hwnd);
}

HWND win32_create_window(int w, int h) {
    return CreateWindowExA(
            WS_EX_OVERLAPPEDWINDOW, "motor_engine", "motor_app",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h,
            0, 0, 0, 0
    );
}

void win32_show_window(HWND hwnd, bool show) {
    ShowWindow(hwnd, show ? SW_SHOW : SW_HIDE);
    UpdateWindow(hwnd);
}

void win32_broadcast() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Sleep(1);
}

void win32_resize_window(HWND hwnd, int w, int h) {
    SetWindowPos(hwnd, NULL, CW_USEDEFAULT, CW_USEDEFAULT, w, h, SWP_SHOWWINDOW);
}

void win32_get_window_size(HWND hwnd, int *w, int *h) {
    RECT rc;
    GetClientRect(hwnd, &rc);

    *w = (int) rc.right - (int) rc.left;
    *h = (int) rc.bottom - (int) rc.top;
}
