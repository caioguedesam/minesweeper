#include "board.h"

void Board::init(GameDifficulty difficulty)
{
	this->difficulty = difficulty;
	switch (difficulty)
	{
	case DIFFICULTY_BEGINNER:
	{
		width = 9;
		height = 9;
	} break;
	case DIFFICULTY_INTERMEDIATE:
	{
		width = 16;
		height = 16;
	} break;
	case DIFFICULTY_EXPERT:
	{
		width = 30;
		height = 16;
	} break;
	default:
		break;
	}

	// TODO_CONTINUE: here
}