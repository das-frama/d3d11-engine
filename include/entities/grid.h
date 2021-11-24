#ifndef MOTOR_GRID_H
#define MOTOR_GRID_H

#include "engine.h"
#include "entity.h"

typedef struct {
    entity_id id;
    vec4 color;
} grid;

entity_id grid_new(int width, int depth, int m, int n);
void grid_delete(entity_id id);

#endif // MOTOR_GRID_H