#ifndef MOTOR_INPUT_H
#define MOTOR_INPUT_H

#include "engine.h"

#define BUTTON_COUNT 256

typedef struct {
    int  x, y;
    bool has_movement;    
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
    bool enabled;
} input;

void input_update(void);
input* input_get(void);

void input_enable(bool enable);

bool input_key_pressed(uchar key_code);
bool input_key_released(uchar key_code);

#endif // MOTOR_INPUT_H