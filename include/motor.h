#ifndef MOTOR_H
#define MOTOR_H

/* Engine includes. */
#include "engine.h"
// #include "asset.h"
#include "graphics.h"
#include "platform.h"
#include "input.h"

#include "assets/renderable.h"
#include "assets/mesh.h"
#include "assets/texture.h"
#include "assets/material.h"
#include "assets/shader.h"

#include "entities/camera.h"
#include "entities/geometry.h"

/* Engine API. */
void motor_init(const char* core_assets_path, const char* name, int w, int h);
void motor_close(void);

bool motor_running(void);
void motor_stop_next(void);

#endif // MOTOR_H