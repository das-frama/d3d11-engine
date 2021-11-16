#include "asset.h"

#include "assets/mesh.h"
#include "assets/shader.h"

#include "vendor/stb_ds.h"

/* Static files. */
static char core_assets_path[PATH_MAX];
static struct {
	char* key;
	asset* value;
}* assets_hash = NULL;

void assets_init(const char* path) {
	strcpy_s(core_assets_path, strlen(path) + 1, path);
}

void assets_close() {
	// free every asset.
	for (int i = 0; i < shlen(assets_hash); i++) {
		assets_unload(assets_hash[i].value);
	}

	// free memory of asset hash table.
	shfree(assets_hash);
}

// assets load.
asset* assets_load(const char* filename) {
	asset* a = malloc(sizeof(asset));
	memset(a, 0, sizeof(asset));
	
	// check if asset is already loaded...
	if (shget(assets_hash, filename)) {
		error("Asset '%s' already loaded", filename);		
	}

	// Check asset by its type.
	if (file_ext_eq(filename, "obj")) {
		// Renderable object.
		mesh* m = mesh_load(filename);
		a->ptr = (void*)m;
	} else {
		error("Loading of %s is not implemented yet.", filename);
	}

	a->loaded = true;

	// store asset in hash table
	shput(assets_hash, filename, a);

	return a;
}

void assets_unload(asset* a) {
	// shdel(assets_hash, a->path);
	free(a->ptr); // TODO (frama): сделать нормальную выгрузку.
	free(a);
}