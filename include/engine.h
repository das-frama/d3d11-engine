#ifndef MOTOR_ENGINE_H
#define MOTOR_ENGINE_H

/* Standard includes. */
#include <stdio.h>   // printf, sprint, snprintf
#include <stdlib.h>  // malloc, free, realpath
#include <string.h>  // strcpy, strcat
#include <assert.h>  // assert
#include <stdbool.h> // bool
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

#ifdef _WIN32 
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

/* Utils include. */
#include "utils/log.h"
#include "math/vec.h"
#include "math/mat.h"

/* Typedefs. */
// typedef short int     sint;
typedef unsigned int  uint;
typedef unsigned char uchar;

/* Engine core. */
void frame_begin(void);
void frame_end(void);
float frame_dt(void);

/* File Helpers. */
const char* file_ext(const char* filename);
bool file_ext_eq(const char* filename, const char* ext2);
const char* file_abs(const char* path);
void* file_load_win32(const char* filename, size_t* len);

/* String Helpers. */
const wchar_t* to_wch(const char *c);


#endif // MOTOR_ENGINE_H