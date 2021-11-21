#ifndef MOTOR_GAME_H
#define MOTOR_GAME_H

#include "engine.h"

void game_start(void);
void game_pause(void);
void game_stop(void);

bool game_should_quit();
void game_should_stop_next();

void game_set_playmode(bool mode);
void game_switch_playmode();
void game_do_one_frame(void);

#endif // MOTOR_GAME_H