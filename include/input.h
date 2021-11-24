#ifndef MOTOR_INPUT_H
#define MOTOR_INPUT_H

#include "engine.h"

#define BUTTON_COUNT 256

typedef struct {
    int  x, y;
    int  dx, dy;
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
    rect screen_area;
    bool enabled;
    bool is_init;
} input;

void input_update(void);
input* input_get(void);

void input_enable(bool enable);
void input_set_screen_area(rect r);

bool input_key_pressed(uchar key_code);
bool input_key_released(uchar key_code);

#endif // MOTOR_INPUT_H