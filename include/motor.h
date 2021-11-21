#ifndef MOTOR_H
#define MOTOR_H

/* Engine includes. */
#include "engine.h"
#include "window.h"
#include "graphics.h"
#include "input.h"
#include "game.h"

#include "assets/renderable.h"
#include "assets/mesh.h"
#include "assets/texture.h"
#include "assets/material.h"
#include "assets/shader.h"

#include "entities/light.h"
#include "entities/camera.h"
#include "entities/grid.h"
#include "entities/skybox.h"

#include "utils/geometry.h"

/* Engine API. */
void motor_init(const char* name, int w, int h);
void motor_close(void);

#endif // MOTOR_H