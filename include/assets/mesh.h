#ifndef MOTOR_MESH_H
#define MOTOR_MESH_H

#include "engine.h"

#include "math/vec.h"
#include "renderer/d3d11_renderer.h"

typedef struct {
	vertex_buffer vb;
	index_buffer ib;
} mesh;

typedef struct {
	vec3 pos;
	vec2 texcoord;
	vec3 normal;
} vertex_mesh;

mesh* mesh_new(void);
void mesh_delete(mesh* m);
void mesh_load_obj(mesh* m, const char* filename);

void load_file_win32(void*, const char*, const int, const char*, char**, size_t*);
// void load_file_posix(void*, const char*, const int, const char*, char**, size_t*);

#endif // MOTOR_MESH_H