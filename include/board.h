#pragma once
#include <stdint.h>

enum GameDifficulty : uint8_t
{
	DIFFICULTY_BEGINNER,
	DIFFICULTY_INTERMEDIATE,
	DIFFICULTY_EXPERT,
};

class Board
{
public:
	void init(GameDifficulty difficulty);
	void destroy();
	
private:
	GameDifficulty difficulty	= DIFFICULTY_BEGINNER;
	uint32_t width				= 0;
	uint32_t height				= 0;
	int* data					= nullptr;
};
