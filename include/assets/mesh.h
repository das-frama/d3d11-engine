#ifndef MOTOR_MESH_H
#define MOTOR_MESH_H

#include "engine.h"

#include "renderer/d3d11_renderer.h"

typedef enum {
	TOPOLOGY_NONE,
	TOPOLOGY_TRIANGLE_LIST,
	TOPOLOGY_LINE_LIST,
} draw_topology;

typedef struct {
	d3d11_vertex_buffer* vb;
	d3d11_index_buffer* ib;
	draw_topology topology;
} mesh;

typedef struct {
    vec3 pos;
    vec2 texcoord;
    vec3 normal;
} vertex;

typedef struct {
	vertex* vertices;
	size_t vertices_count;

	uint* indices;
	size_t indices_count;
} mesh_data;

mesh* mesh_new(void);
mesh* mesh_new_load(const char* filename);
mesh* mesh_new_data(mesh_data* md);
void mesh_delete(mesh* m);
void mesh_data_set(mesh*m, mesh_data* md);
void mesh_data_delete(mesh_data* md);

#endif // MOTOR_MESH_H