#include "asset.h"

#include "assets/mesh.h"
#include "assets/shader.h"

#include "vendor/stb_ds.h"

static fpath core_asset_path;
static struct {
	char* key;
	asset* value;
}* assets_ht = NULL;

void asset_init(const char* path) {
	core_asset_path = fpath_new(path);
}

void asset_close() {
	// free memory of asset hash table
	shfree(assets_ht);
}

asset* asset_load(const char* asset_path) {
	asset* a = malloc(sizeof(asset));
	memset(a, 0, sizeof(asset));
	
	a->path = abs_path(fpath_new(asset_path));

	// check if asset is already loaded...
	if (shget(assets_ht, a->path.str)) {
		error("Asset '%s' already loaded.", asset_path);		
	}

	if (strcmp(file_extension(a->path.str), "obj") == 0) {
		mesh* m = mesh_new();
		mesh_load_obj(m, a->path.str);
		a->ptr = (void*)m;
	} else if (strcmp(file_extension(a->path.str), "fx") == 0) {
		shader* s = shader_new();
		shader_load_fx(s, a->path.str);
		a->ptr = (void*)s;
	} else if (strcmp(file_extension(a->path.str), "hlsl") == 0) {
		shader* s = shader_new();
		// shader_load_hlsl(s, a->path.str, "vsmain");
		// shader_load_hlsl(s, a->path.str, );

		a->ptr = (void*)s;
	} else {
		error("Loading of %s is not implemented yet.", a->path.str);
	}

	a->loaded = true;

	// store asset in hash table
	shput(assets_ht, a->path.str, a);

	return a;
}

void asset_unload(asset* a) {
	shdel(assets_ht, a->path.str);
	free(a->ptr); // TODO (frama): сделать нормальную выгрузку.
	free(a);
}

const char* file_extension(const char* file_name) {
    return strrchr(file_name, '.') + 1;
}

fpath abs_path(fpath local_asset_path) {
	fpath out;

	/* Concat path. */
	char buf[PATH_MAX] = "";
	strcat_s(buf, PATH_MAX, core_asset_path.str);
	strcat_s(buf, PATH_MAX, SEPARATOR);
	strcat_s(buf, PATH_MAX, local_asset_path.str);

	/* Fetch absolute path. */
	char full[PATH_MAX] = "";
	if (!_fullpath(full, buf, PATH_MAX)) {
		error("asset :: asset load error: %s", local_asset_path.str);
	}
	strcpy_s(out.str, PATH_MAX, full);

	return out;
}
