#ifndef MOTOR_INPUT_H
#define MOTOR_INPUT_H

#include "engine.h"

#define BUTTON_COUNT 256

typedef struct {
    int  x, y;
    bool left_pressed,  right_pressed;
    bool left_released, right_released;
} mouse;

typedef struct {
    bool pressed;
    bool released;
} button;

typedef struct {
    mouse  mouse;
    button keys[BUTTON_COUNT];
} input;

void input_update(input* in);

#endif // MOTOR_INPUT_H