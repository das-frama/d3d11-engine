#ifndef MOTOR_MESH_H
#define MOTOR_MESH_H

#include "engine.h"

#include "math/vec.h"
#include "renderer/d3d11_renderer.h"

typedef struct {
	vertex_buffer* vb;
	index_buffer* ib;
} mesh;

// mesh* mesh_new(void);
mesh* mesh_load(const char* filename);
void mesh_unload(mesh* m);

#endif // MOTOR_MESH_H