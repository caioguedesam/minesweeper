#include "board.h"
#include "random.h"
#include <string.h>

void Tile::reset()
{
	revealed		= false;
	has_bomb		= false;
	has_flag		= false;
	has_radar		= false;
	adjacent_bombs	= -1;
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
	memset(bomb_locations, 0, sizeof(bomb_locations));
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

	// Simples amostragem aleat?ria com white noise para coloca??o de bombas
	while (bombs)
	{
		int x = dist_uniform(0, height);
		int y = dist_uniform(0, width);
		Tile& tile = grid[x][y];
		if (!tile.has_bomb)
		{
			tile.has_bomb = true;
			bomb_locations[bombs - 1][0] = x;
			bomb_locations[bombs - 1][1] = y;
			bombs--;
		}
	}

	// Fazendo cache de bombas adjacentes por tile para eliminar checagens redundantes
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			Tile& tile = grid[x][y];
			int adj_bombs = get_adjacent_bombs(x, y);
			if (adj_bombs >= 0)
			{
				tile.adjacent_bombs = adj_bombs;
			}
		}
	}

	// Colocando um radar como b?nus para o jogador em posi??o vazia aleat?ria.
	while (true)
	{
		int x = dist_uniform(0, height);
		int y = dist_uniform(0, width);
		Tile& tile = grid[x][y];
		if (!tile.has_bomb && get_adjacent_bombs(x, y) == 0)
		{
			tile.has_radar = true;
			break;
		}
	}
}

bool Board::is_on_win_state()
{
	return !revealed_bomb && revealed_tiles == (width * height) - bomb_count;
}

bool Board::is_on_lose_state()
{
	return revealed_bomb;
}

bool Board::is_on_bounds(int x, int y)
{
	return x >= 0 && y >= 0 && x < height && y < width;
}

int Board::get_adjacent_bombs(int x, int y)
{
	if (!is_on_bounds(x, y)) return -1;
	Tile& tile = grid[x][y];
	if (tile.has_bomb) return -1;
	if (tile.adjacent_bombs >= 0) return tile.adjacent_bombs;	// Cache de bombas adjacentes para n?o repetir checagens

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