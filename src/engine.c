#include "engine.h"

#define STB_DS_IMPLEMENTATION
#include "vendor/stb_ds.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "vendor/tinyobj_loader_c.h"

static char frame_rate_string_var[12];
static int frame_rate_var = 0;
static float frame_delta_time = 0.0f;
static uint frame_start_time = 0;
static uint frame_end_time = 0;
static const float frame_update_rate = 0.5f;
static short frame_counter = 0;
static float frame_acc_time = 0.0f;
static double frame_global_time = 0.0f;

void frame_begin() {
    frame_start_time = GetTickCount64();
}

void frame_end() {
    frame_end_time = GetTickCount64();

    frame_delta_time = ((float) (frame_end_time - frame_start_time) / 1000.0f);
    frame_acc_time += frame_delta_time;
    frame_global_time += frame_delta_time;
    frame_counter++;

    if (frame_acc_time > frame_update_rate) {
        frame_rate_var = round((float) frame_counter / frame_acc_time);
        frame_counter = 0;
        frame_acc_time = 0.0f;
    }

    sprintf(frame_rate_string_var, "%i", frame_rate_var);
}

float frame_dt() {
    return frame_delta_time;
}

float frame_total_time() {
    return frame_global_time;
}

const char* file_ext(const char* filename) {
    return strrchr(filename, '.') + 1;
}

bool file_ext_eq(const char* filename, const char* ext2) {
    return strcmp(file_ext(filename), ext2) == 0;
}

const char* file_abs(const char* filename) {
    char buf[PATH_MAX];
    if (!_fullpath(buf, filename, PATH_MAX)) {
        error("file %s error", filename);
    }

    size_t str_size = strlen(buf) + 1;
    char* res = malloc(sizeof(char) * str_size);
    strcpy_s(res, str_size, buf);

    return res;
}

const wchar_t* to_wch(const char *c) {
    const size_t c_size = strlen(c) + 1;
    wchar_t* wc = malloc(sizeof(wchar_t) * c_size);
    mbstowcs(wc, c, c_size);

    return wc;
}

void* file_load_win32(const char* filename, size_t* len) {
    HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, 
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (file == INVALID_HANDLE_VALUE) { // Model may not have materials.
        error("error file");
    }

    HANDLE fmap = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
    assert(fmap != INVALID_HANDLE_VALUE);

    LPVOID fmap_view = MapViewOfFile(fmap, FILE_MAP_READ, 0, 0, 0);
    assert(fmap_view != NULL);

    *len = (size_t)GetFileSize(file, NULL);

    CloseHandle(fmap);
    CloseHandle(file);

    return (void*)fmap_view;
}