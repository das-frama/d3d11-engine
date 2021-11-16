#ifndef MOTOR_assets_H
#define MOTOR_assets_H

#include "engine.h"

typedef enum {
	ASSET_
} asset_type;

typedef struct {
	void* ptr;
	bool loaded;
	asset_type type;
} asset;

/* Asset API. */
void assets_init(const char* core_assets_path);
void assets_close(void);

asset* assets_load(const char* filename);
void assets_unload(asset* a);

#endif // MOTOR_assets_H