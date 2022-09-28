#include "board.h"
#include "random.h"
#include <string.h>

void Board::clear()
{
	memset(grid, 0, sizeof(grid));
	width = 0;
	height = 0;
}

void Board::init(int w, int h, int bombs)
{
	clear();
	width = w;
	height = h;

	// Simple white noise randomization for bomb placement
	while (bombs)
	{
		int x = dist_uniform(0, width + 1);
		int y = dist_uniform(0, height + 1);
		Tile& tile = grid[x][y];
		if (!tile.has_bomb)
		{
			tile.has_bomb = true;
			bombs--;
		}
	}
}

bool Board::is_on_win_state()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Tile& tile = grid[i][j];
			if (!tile.revealed && !tile.has_bomb) return false;
			if (tile.revealed && tile.has_bomb) return false;
		}
	}
	return true;
}

bool Board::is_on_lose_state()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Tile& tile = grid[i][j];
			if (tile.has_bomb && tile.revealed) return true;
		}
	}
	return false;
}

bool Board::is_on_bounds(int x, int y)
{
	return x >= 0 && y >= 0 && x < width && y < height;
}

void Board::reveal_tile(int x, int y)
{
	if (!is_on_bounds(x, y)) return;

	Tile& tile = grid[x][y];
	if (tile.revealed || tile.has_bomb) return;

	tile.revealed = true;
	
	if (get_adjacent_bombs(x, y) == 0)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				reveal_tile(x + i, y + j);
			}
		}
	}
}

int Board::get_adjacent_bombs(int x, int y)
{
	if (!is_on_bounds(x, y)) return -1;
	Tile& tile = grid[x][y];
	if (tile.has_bomb) return -1;

	int bomb_count = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (!is_on_bounds(x + i, y + j) || (i == 0 && j == 0)) continue;
			Tile& neighbor = grid[x + i][y + j];
			if (neighbor.has_bomb)
			{
				bomb_count++;
			}
		}
	}
	return bomb_count;
}

void Board::toggle_flag(int x, int y)
{
	if (!is_on_bounds(x, y)) return;
	grid[x][y].has_flag = !grid[x][y].has_flag;
}