#pragma once
#include "game.h"

/*
	TODO: CLI version
	TODO: Immediate mode GUI version
*/

void ui_clear();
void ui_render_game(Game* game);
void ui_poll_input(Game* game);