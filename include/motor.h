#ifndef MOTOR_H
#define MOTOR_H

/* Engine includes. */
#include "engine.h"
#include "asset.h"
#include "graphics.h"

#include "renderer/d3d11_renderer.h"
#include "platform/win32_platform.h"

/* Engine API. */
void motor_init(const char* core_asset_path, int w, int h);
void motor_close(void);
bool motor_running(void);
void motor_process_events();

#endif // MOTOR_H