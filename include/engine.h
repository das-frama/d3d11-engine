#ifndef MOTOR_ENGINE_H
#define MOTOR_ENGINE_H

/* Defines. */
#define FILE_PATH_MAX 512

/* Standard includes. */
#include <stdio.h> // printf, sprint, snprintf
#include <stdlib.h> // malloc, free
#include <string.h> // strcpy

/* Windows includes. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* DirectX includes. */
#define D3D11_NO_HELPERS
#define COBJMACROS
#include <d3d11_1.h>

/* Utils. */
#include "utils/utils.h"
#include "utils/log.h"

/* Common structs. */
typedef struct {
	char ptr[FILE_PATH_MAX];
} file_path;

/* Engine core. */
void frame_begin(void);
void frame_end(void);

/* Helpers. */
file_path file_path_new(const char* path);

#endif // MOTOR_ENGINE_H