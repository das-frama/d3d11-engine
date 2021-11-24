#ifndef MOTOR_GEOMETRY_H
#define MOTOR_GEOMETRY_H

#include "engine.h"

#include "assets/mesh.h"

mesh_data* generate_grid(int width, int depth, int m, int n);
mesh_data* generate_sphere(float radius, size_t slice_count, size_t stack_count);

#endif // MOTOR_GEOMETRY_H