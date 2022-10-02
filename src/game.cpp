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

void Game::action_reveal_tile(int x, int y)
{
	if (state != GameState::PLAYING || !board.is_on_bounds(x, y)) return;

	Tile& tile = board.grid[x][y];
	if (tile.revealed) return;

	tile.revealed = true;
	board.revealed_tiles++;
	if (tile.has_bomb)
	{
		board.revealed_bomb = true;
		return;	// kaboom!
	}

	if (board.get_adjacent_bombs(x, y) == 0)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				action_reveal_tile(x + i, y + j);
			}
		}
	}
}

void Game::action_toggle_flag(int x, int y)
{
	if (state != GameState::PLAYING) return;

	if (!board.is_on_bounds(x, y)) return;
	board.grid[x][y].has_flag = !board.grid[x][y].has_flag;
}

bool Game::is_won()
{
	return won_game;
}

void Game::exit()
{
	is_running = false;
}