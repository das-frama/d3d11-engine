#ifndef MOTOR_ENGINE_H
#define MOTOR_ENGINE_H

/* Standard includes. */
#include <stdio.h> // printf, sprint, snprintf
#include <stdlib.h> // malloc, free, realpath
#include <string.h> // strcpy, strcat
#include <assert.h> // assert

/* Windows includes. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* DirectX includes. */
#define D3D11_NO_HELPERS
#define COBJMACROS
#include <d3d11_1.h>
#include <dxgi1_6.h>

/* Utils. */
#include "utils/utils.h"
#include "utils/log.h"

/* Math. */
#include "math/vec.h"
#include "math/mat.h"

/* Defines. */
#define PATH_MAX 512

#ifdef _WIN32 
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

/* Common structs. */
typedef struct {
    char str[PATH_MAX];
} fpath;

/* Engine core. */
void frame_begin(void);
void frame_end(void);
float frame_dt(void);

/* Helpers. */
fpath fpath_new(const char* path);

#endif // MOTOR_ENGINE_H