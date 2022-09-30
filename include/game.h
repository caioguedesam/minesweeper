#pragma once
#include "board.h"

enum class GameState
{
	STARTING,
	PLAYING,
	ENDED,
};

enum class GameDifficulty
{
	BEGINNER		= 1,
	INTERMEDIATE	= 2,
	EXPERT			= 3,
};

enum class GameAction
{
	REVEAL_TILE,
	TOGGLE_FLAG,
};

struct Game
{
	bool is_running = true;
	GameState state	= GameState::STARTING;
	GameDifficulty difficulty = GameDifficulty::BEGINNER;
	bool won_game = false;

	Board board;

	void reset();								// ENDED	-> STARTING
	void start(GameDifficulty new_difficulty);	// STARTING -> PLAYING
	void process_action(GameAction action, int x, int y);
	void check_end_condition();					// PLAYING	-> ENDED

	bool is_won();
};