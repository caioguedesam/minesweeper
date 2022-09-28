#pragma once
#include "board.h"

enum class GameState
{
	STARTING	= 1,
	PLAYING		= 2,
	ENDED		= 3,
};

enum class GameDifficulty
{
	BEGINNER,
	INTERMEDIATE,
	EXPERT,
};

enum class GameAction
{
	REVEAL_TILE,
	TOGGLE_FLAG,
};

struct Game
{
	GameState state;
	GameDifficulty difficulty;
	bool won_game = false;

	Board board;

	void reset();								// ENDED	-> STARTING
	void start(GameDifficulty new_difficulty);	// STARTING -> PLAYING
	void process_action(GameAction action, int x, int y);
	void check_end_condition();					// PLAYING	-> ENDED

	bool is_won();
};