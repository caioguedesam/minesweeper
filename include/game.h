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

struct Game
{
	bool is_running = true;
	GameState state	= GameState::STARTING;
	GameDifficulty difficulty = GameDifficulty::BEGINNER;
	bool won_game = false;

	Board board;

	void reset();								// ENDED	-> STARTING
	void start(GameDifficulty new_difficulty);	// STARTING -> PLAYING
	void check_end_condition();					// PLAYING	-> ENDED

	void action_reveal_tile(int x, int y);
	void action_toggle_flag(int x, int y);

	bool is_won();
	void exit();
};