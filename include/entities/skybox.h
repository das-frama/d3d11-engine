#ifndef MOTOR_SKYBOX_H
#define MOTOR_SKYBOX_H

#include "engine.h"

#include "assets/renderable.h"
#include "entities/camera.h"

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
    vec4 color;
    vec4 color1;
} skybox_constant;

typedef struct {
    renderable* rnd;
    skybox_constant cc;
} skybox;

skybox* skybox_new();
void skybox_delete(skybox* sky);

void skybox_update(skybox* sky, const camera* cam);
void skybox_draw(const skybox* sky);

#endif // MOTOR_SKYBOX_H