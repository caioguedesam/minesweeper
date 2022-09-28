#pragma once
#include <stdint.h>

#define BOARD_MAX_WIDTH		30
#define BOARD_MAX_HEIGHT	16

struct Tile
{
	bool revealed = false;
	bool has_bomb = false;
	bool has_flag = false;
	int adjacent_bombs = -1;

	void reset();
};

struct Board
{
	int width = 0;
	int height = 0;
	Tile grid[BOARD_MAX_WIDTH][BOARD_MAX_HEIGHT];
	int bomb_count = 0;
	int revealed_tiles = 0;
	bool revealed_bomb = false;

	void init(int w, int h, int bombs);

	void clear();
	bool is_on_win_state();
	bool is_on_lose_state();

	bool is_on_bounds(int x, int y);

	void reveal_tile(int x, int y);
	int get_adjacent_bombs(int x, int y);
	void toggle_flag(int x, int y);
};
