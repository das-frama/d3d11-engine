#include "assets/renderable.h"

#include "utils/geometry.h"
#include "vendor/stb_ds.h"
#include "shaders/mesh_vs.h"
#include "shaders/mesh_ps.h"

__declspec(align(16))
typedef struct {
    mat4 world;
    mat4 view;
    mat4 proj;
} constant;

renderable* renderable_new(void) {
	renderable* r = calloc(1, sizeof(renderable));
	return r;	
}

void renderable_delete(renderable* r, bool del_meshes, bool del_mat) {
	// unload meshes.
	if (del_meshes) {
		for (int i = 0; i < r->mesh_size; i++) {
			mesh_delete(r->meshes[i]);
		}
	}
	arrfree(r->meshes);
	r->meshes = NULL;
	r->mesh_size = 0;

	// unload material.
	if (del_mat) {
		material_delete(r->material);
	}
	r->material = NULL;
	
	free(r);
}

void renderable_delete_all(renderable* r) {
	renderable_delete(r, true, true);
}

void renderable_add_mesh(renderable* r, mesh* m) {
	arrput(r->meshes, m);
	r->mesh_size = arrlen(r->meshes);
}

renderable* renderable_sphere(float radius) {
	renderable* r = renderable_new();

	// Generate and set sphere mesh.
	mesh_data* md = generate_sphere(radius, 30, 30);
    mesh* msh = mesh_new_data(md);
    mesh_data_delete(md);
    renderable_add_mesh(r, msh);

    // Set default material.
    shader* vs = shader_new_byte(g_mesh_vs, _countof(g_mesh_vs), SHADER_TYPE_VS);
    shader* ps = shader_new_byte(g_mesh_ps, _countof(g_mesh_ps), SHADER_TYPE_PS);
    constant cc = {0};
    r->material = material_new_(vs, ps, NULL, &cc, sizeof(constant));

    return r;
}