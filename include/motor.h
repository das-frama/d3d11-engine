#ifndef MOTOR_H
#define MOTOR_H

/* Defines. */

/* Standart includes. */
#include <stdio.h>

/* Engine includes. */
#include "utils/utils.h"
#include "utils/log.h"

void motor_init(const char* core_asset_path, int w, int h);
void motor_close(void);

#endif // MOTOR_H
