#ifndef MOTOR_H
#define MOTOR_H

/* Engine includes. */
#include "engine.h"
#include "asset.h"
#include "graphics.h"

#include "assets/mesh.h"
#include "assets/texture.h"
#include "assets/material.h"
#include "assets/shader.h"

/* Macros. */
#define process_events() { \
	while (win32_has_messages()) win32_process_messages(); \
}

/* Engine API. */
void motor_init(const char* core_assets_path, const char* name, int w, int h);
void motor_close(void);
bool motor_running();

#endif // MOTOR_H