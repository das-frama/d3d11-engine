#ifndef MOTOR_ENTITY_H
#define MOTOR_ENTITY_H

#include "engine.h"

// #include "assets/renderable.h"

// typedef struct {
//     vec3 position;
//     vec3 scale;
//     vec4 rotation;

//     bool active;
//     bool recieve_shadows;
//     bool cast_shadows;

//     renderable* renderable;
// } entity;

void entity_new(const char* type, ...);

#endif // MOTOR_ENTITY_H