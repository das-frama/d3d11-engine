#ifndef MOTOR_H
#define MOTOR_H

/* Engine includes. */
#include "engine.h"
#include "graphics.h"
#include "platform.h"
#include "input.h"

#include "assets/renderable.h"
#include "assets/mesh.h"
#include "assets/texture.h"
#include "assets/material.h"
#include "assets/shader.h"

#include "entities/light.h"
#include "entities/camera.h"
#include "entities/grid.h"

#include "utils/geometry.h"

/* Engine API. */
void motor_init(const char* name, int w, int h);
void motor_close(void);

bool motor_running(void);
void motor_stop_next(void);

#endif // MOTOR_H