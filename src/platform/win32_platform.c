#include "platform/win32_platform.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "vendor/nuklear.h"
#include "vendor/nuklear_d3d11.h"

bool win32_should_quit = false;

//static MSG g_msg;
static HWND g_window = {0};

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_CLOSE:
        case WM_DESTROY:
            win32_should_quit = true;
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            // if (IsWindow(g_window)) d3d11_reload_buffers();
            break;
    }

    if (nk_d3d11_handle_event(hwnd, msg, wparam, lparam)) {
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void win32_init() {
    WNDCLASSEX wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = "MyWindowClass";
    wc.lpszMenuName = "";
    wc.style = 0;
    wc.lpfnWndProc = &wnd_proc;

    if (!RegisterClassEx(&wc)) {
        error("RegisterClassEx error");
    }
}

void win32_close() {
    DestroyWindow(g_window);
    UnregisterClass("MyWindowClass", NULL);
}

void win32_create(int w, int h) {
    g_window = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW, "MyWindowClass", "My Game",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h,
            0, 0, 0, 0);
}

void win32_show(bool show) {
    ShowWindow(g_window, show ? SW_SHOW : SW_HIDE);
    UpdateWindow(g_window);
}

HWND win32_window() {
    return g_window;
}

void win32_set_title(const char* title) {
    SetWindowTextA(g_window, title);
}

void win32_resize(int w, int h) {
    SetWindowPos(g_window, 0, CW_USEDEFAULT, CW_USEDEFAULT, w, h, SWP_NOZORDER|SWP_NOMOVE);
}

void win32_size(int *w, int *h) {
    RECT rc;
    GetClientRect(g_window, &rc);

    *w = (int) rc.right - (int) rc.left;
    *h = (int) rc.bottom - (int) rc.top;
}

rect win32_client_rect() {
    RECT rc = {0};
    GetClientRect(g_window, &rc);
    ClientToScreen(g_window, (LPPOINT)&rc.left);
    ClientToScreen(g_window, (LPPOINT)&rc.right);
    return rect_new(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

void win32_process_messages() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Sleep(1);
}

void win32_sleep(uint ms) {
    Sleep(ms);     
}

vec2 win32_mouse_pos() {
    POINT pos = {0};
    GetCursorPos(&pos);

    return vec2_new(pos.x, pos.y);
}

void win32_keyboard_state(uchar* key_state) {
    GetKeyboardState(key_state);
}

void win32_set_mouse_pos(float x, float y) {
    SetCursorPos(x, y);
}

void win32_show_mouse(bool show) {
    ShowCursor(show);
    UpdateWindow(g_window);
}