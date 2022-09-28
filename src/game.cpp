#include "game.h"

void Game::reset()
{
	if (state != GameState::ENDED) return;
	state = GameState::STARTING;
}

void Game::start(GameDifficulty new_difficulty)
{
	if (state != GameState::STARTING) return;
	difficulty = new_difficulty;
	won_game = false;

	switch (difficulty)
	{
	case GameDifficulty::BEGINNER:
	{
		board.init(9, 9, 10);
	} break;
	case GameDifficulty::INTERMEDIATE:
	{
		board.init(16, 16, 40);
	} break;
	case GameDifficulty::EXPERT:
	{
		board.init(30, 16, 99);
	} break;
	default: break;
	}

	state = GameState::PLAYING;
}

void Game::process_action(GameAction action, int x, int y)
{
	if (state != GameState::PLAYING) return;

	switch (action)
	{
	case GameAction::REVEAL_TILE:
	{
		board.reveal_tile(x, y);
	} break;
	case GameAction::TOGGLE_FLAG:
	{
		board.toggle_flag(x, y);
	} break;
	default: break;
	}
}

void Game::check_end_condition()
{
	if (state != GameState::PLAYING) return;

	if (board.is_on_win_state())
	{
		won_game = true;
		state = GameState::ENDED;
	}
	else if (board.is_on_lose_state())
	{
		state = GameState::ENDED;
	}
}

bool Game::is_won()
{
	return won_game;
}