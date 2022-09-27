#pragma once
#include "stdio.h"

class Game
{
public:
	void init();
	void destroy();

	void poll_input();
	void update();
	void render();

private:
	bool is_running = true;
};