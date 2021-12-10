#ifndef MOTOR_ENGINE_H
#define MOTOR_ENGINE_H

/* Standard includes. */
#include <stdio.h>   // printf, sprint, snprintf
#include <stdlib.h>  // malloc, free, realpath
#include <string.h>  // strcpy, strcat
#include <assert.h>  // assert
#include <stdbool.h> // bool
#include <stdint.h>  // for short aliases
// #include <tgmath.h>
#include <math.h> // cos, sin, pow

/* Windows includes. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* DirectX includes. */
#define D3D11_NO_HELPERS
#define COBJMACROS
#include <d3d11_1.h>
#include <dxgi1_6.h>

/* Defines. */
#define PATH_MAX 512
#define PI 3.14159265358
#define PI2 6.28318530718

#ifdef _WIN32 
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

/* Utils include. */
#include "utils/log.h"
#include "math/vec.h"
#include "math/mat.h"
#include "math/rect.h"

/* Typedefs. */
typedef unsigned int  uint;
typedef unsigned char uchar;
typedef unsigned char byte;

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef bool b8;

/* Common structs. */
typedef struct {
    char buf[PATH_MAX];
} filepath;

/* Engine core. */
void frame_begin(void);
void frame_end(void);
float frame_dt(void);
float frame_total_time(void);

/* Macroces. */
#define to_rad(n) (n * 0.017453f)

/* File Helpers. */
filepath fp(const char* filename);
const char* file_ext(const char* filename);
bool file_ext_eq(const char* filename, const char* ext2);
const char* file_abs(const char* path);
void* file_load_win32(const char* filename, size_t* len);

/* String Helpers. */
const wchar_t* to_wch(const char *c);

#endif // MOTOR_ENGINE_H