#include "asset.h"

#include "assets/mesh.h"
#include "assets/shader.h"

#include "vendor/stb_ds.h"

static char core_assets_path[PATH_MAX];
static struct {
	char* key;
	asset* value;
}* assets_hash = NULL;

void assets_init(const char* path) {
	strncpy(core_assets_path, path, PATH_MAX - 1);
}

void assets_close() {
	// free every asset.
	for (int i = 0; i < shlen(assets_hash); i++) {
		assets_unload(assets_hash[i].value);
	}

	// free memory of asset hash table.
	shfree(assets_hash);
}

asset* assets_load(const char* filename) {
	asset* a = malloc(sizeof(asset));
	memset(a, 0, sizeof(asset));
	
	a->path = file_abs(filename);

	// check if asset is already loaded...
	if (shget(assets_hash, a->path)) {
		error("Asset '%s' already loaded", filename);		
	}

	// renderable.
	if (strcmp(file_ext(filename), "obj") == 0) {
		mesh* m = mesh_load(filename);
		a->ptr = (void*)m;
	} else {
		error("Loading of %s is not implemented yet.", filename);
	}

	a->loaded = true;

	// store asset in hash table
	shput(assets_hash, a->path, a);

	return a;
}

void assets_unload(asset* a) {
	shdel(assets_hash, a->path);
	free(a->ptr); // TODO (frama): сделать нормальную выгрузку.
	free(a);
}