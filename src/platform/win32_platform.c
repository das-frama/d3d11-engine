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
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = &wnd_proc;

    if (!RegisterClassEx(&wc)) {
        return false;
    }

    return true;
}

void win32_close(HWND hwnd) {
    DestroyWindow(hwnd);
}

HWND win32_create_window(int w, int h) {
    return CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"My Game",
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
