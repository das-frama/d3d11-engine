#ifndef MOTOR_ASSET_H
#define MOTOR_ASSET_H

#include "engine.h"

#include "assets/mesh.h"

typedef struct {
	fpath path;
	void* ptr;
	bool loaded;
} asset;

/* Asset API. */
void asset_init(const char* core_asset_path);
void asset_close(void);

asset* asset_load(const char* asset_path);
void asset_unload(asset* a);

/* Helpers. */
const char* file_extension(const char* file_name);
fpath abs_path(fpath path);

#endif // MOTOR_ASSET_H