#ifndef MOTOR_H
#define MOTOR_H

/* Defines. */
#define WIN32_LEAN_AND_MEAN

/* Standart includes. */

/* Engine includes. */
#include "utils/utils.h"
#include "utils/log.h"
#include "graphics.h"

/* Engine API. */
void motor_init(const char* core_asset_path, int w, int h);
void motor_close(void);

bool motor_running();
void motor_process_events();

void motor_begin_frame();
void motor_end_frame();


#endif // MOTOR_H
