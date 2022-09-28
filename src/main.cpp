/*

Game architecture:
	> UI
		> User input
		> Has ref to game, calls set_game_state on button press
		> Renders differently based on game state
	> Game
		> Keeps internal game state
		> Game loop: change state if needed, wait for input, process input
		> Has ref to board, alters board when processing input
	> Board
		> Matrix of tiles

*/

#include "game.h"

int main()
{
	return 0;
}