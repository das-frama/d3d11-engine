#ifndef MOTOR_SKYBOX_H
#define MOTOR_SKYBOX_H

#include "engine.h"

#include "assets/renderable.h"

typedef struct {
    renderable* rnd;
    int width, height;
} skybox;

skybox* skybox_new(const char* texture_path);
void skybox_delete(skybox* sb);

#endif // MOTOR_SKYBOX_H