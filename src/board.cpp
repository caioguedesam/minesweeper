#include "board.h"
#include "random.h"
#include <string.h>

void Tile::reset()
{
	revealed = false;
	has_bomb = false;
	has_flag = false;
	adjacent_bombs = -1;
}

void Board::clear()
{
	for (int i = 0; i < BOARD_MAX_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_MAX_WIDTH; j++)
		{
			grid[i][j].reset();
		}
	}
	width = 0;
	height = 0;
	bomb_count = 0;
	revealed_tiles = 0;
	revealed_bomb = false;
}

void Board::init(int w, int h, int bombs)
{
	clear();
	width = w;
	height = h;
	bomb_count = bombs;

	// Simples amostragem aleatória com white noise para colocação de bombas
	while (bombs)
	{
		int x = dist_uniform(0, height);
		int y = dist_uniform(0, width);
		Tile& tile = grid[x][y];
		if (!tile.has_bomb)
		{
			tile.has_bomb = true;
			bombs--;
		}
	}

	// Fazendo cache de bombas adjacentes por tile para eliminar checagens redundantes
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Tile& tile = grid[i][j];
			int adj_bombs = get_adjacent_bombs(i, j);
			if (adj_bombs >= 0)
			{
				tile.adjacent_bombs = adj_bombs;
			}
		}
	}
}

bool Board::is_on_win_state()
{
	return revealed_tiles == (width * height) - bomb_count;
}

bool Board::is_on_lose_state()
{
	return revealed_bomb;
}

bool Board::is_on_bounds(int x, int y)
{
	return x >= 0 && y >= 0 && x < height && y < width;
}

void Board::reveal_tile(int x, int y)
{
	if (!is_on_bounds(x, y)) return;

	Tile& tile = grid[x][y];
	if (tile.revealed) return;

	tile.revealed = true;
	revealed_tiles++;
	if (tile.has_bomb)
	{
		revealed_bomb = true;
		return;	// kaboom
	}
	
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
	if (tile.adjacent_bombs >= 0) return tile.adjacent_bombs;	// Cache de bombas adjacentes para não repetir checagens

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