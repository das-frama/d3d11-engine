#include "assets/renderable.h"

#include "vendor/stb_ds.h"

renderable* renderable_new(void) {
	renderable* r = malloc(sizeof(renderable));
	memset(r, 0, sizeof(renderable));

	return r;	
}

void renderable_delete(renderable* r) {
	// unload meshes.
	for (int i = 0; i < r->mesh_size; i++) {
		mesh_unload(r->meshes[i]);
	}
	arrfree(*r->meshes);
	r->mesh_size = 0;

	// unload material.
	material_unload(r->material);
	
	free(r);
}

void renderable_add_mesh(renderable* r, mesh* m) {
	arrput(r->meshes, m);
	r->mesh_size = arrlen(r->meshes);
}

void renderable_add_material(renderable* r, material* mat) {
	r->material = mat;
}