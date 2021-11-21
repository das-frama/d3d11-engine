#ifndef MOTOR_LIGHT_H
#define MOTOR_LIGHT_H

#include "engine.h"

typedef struct {
    vec3 position;
    vec3 target;

    vec3 color;

    int index;
} light;

light* light_new(void);
light* light_new_position(vec3 pos);
void light_delete(light* l);
void light_delete_all(void);

void light_update(light* l);

#endif // MOTOR_LIGHT_H