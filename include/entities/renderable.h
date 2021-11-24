#ifndef MOTOR_RENDARABLE_H
#define MOTOR_RENDARABLE_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/material.h"
#include "entities/camera.h"


renderable* renderable_new(void);
void renderable_delete(renderable* r);
void renderable_delete_only(renderable* r, bool del_meshes, bool del_mat);

void renderable_add_mesh(renderable* r, mesh* m);
void renderable_set_material(renderable* r, material* mat);

// void renderable_update(renderable* r, camera* cam);

/* Usefull shortcuts. */
renderable* renderable_sphere(float radius);
renderable* renderable_cube(float radius);

#endif // MOTOR_RENDARABLE_H~