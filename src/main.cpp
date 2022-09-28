/*

Main loop w/ CLI:
	> UI clear
	> UI render game state
		> state dependant
	> UI polls for user input (waits on scanf)
		> when input is read, process input for given game state

TODO: main loop w/ GUI
*/

#include "ui.h"

int main()
{
	Game game;

	while (true)
	{
		ui_clear();
		ui_render_game(&game);
		ui_poll_input(&game);
		game.check_end_condition();
	}

	return 0;
}