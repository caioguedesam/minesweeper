#pragma once
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLI_PRINT(MSG, ...)\
do {\
	printf(MSG, __VA_ARGS__);\
} while(false)

#define CLI_PRINTLN(MSG, ...)\
do {\
	printf(MSG, __VA_ARGS__);\
	printf("\n");\
} while(false)

void ui_clear()
{
	system("cls || clear");		// Clear on linux and windows hack :p
}

void ui_render_board(Game* game)
{
	Board& board = game->board;

	CLI_PRINT("%*c", 5, ' ');

	for (int j = 0; j < board.height; j++)
	{
		CLI_PRINT("%*c", 5, j + '0');
	}
	CLI_PRINTLN("");
	for (int j = 0; j <= board.height; j++)
	{
		CLI_PRINT("%*c", 5, '_');
	}
	CLI_PRINTLN("");

	for (int i = 0; i < board.width; i++)
	{
		CLI_PRINT("%*c|", 4, i + '0');
		for (int j = 0; j < board.height; j++)
		{
			Tile& tile = board.grid[i][j];
			char visual = '.';
			if (tile.revealed)
			{
				if (tile.has_bomb) visual = 'X';
				else visual = tile.adjacent_bombs + '0';
			}
			else if (tile.has_flag)
			{
				visual = 'F';
			}

			CLI_PRINT("%*c", 5, visual);
		}
		CLI_PRINTLN("");
		CLI_PRINTLN("");
	}
}

void ui_render_game(Game* game)
{
	switch (game->state)
	{
	case GameState::STARTING:
	{
		CLI_PRINTLN("============== Campo Minado ==============");
		CLI_PRINTLN("========= Selecionar Dificuldade =========");
		CLI_PRINTLN("		1. Iniciante (9x9 - 10 bombas)");
		CLI_PRINTLN("		2. Intermedi�rio (16x16 - 40 bombas)");
		CLI_PRINTLN("		3. Expert (16x30 - 99 bombas)");
		CLI_PRINTLN("==========================================");
		CLI_PRINTLN("		Digite o n�mero da dificuldade desejada:");
	} break;
	case GameState::PLAYING:
	{
		CLI_PRINTLN("==========================================");
		ui_render_board(game);
		CLI_PRINTLN("==========================================");
	} break;
	case GameState::ENDED:
	{
		CLI_PRINTLN("============== FIM DE JOGO! ==============");
		if (game->is_won())
		{
			CLI_PRINTLN("		Voc� GANHOU!");
		}
		else
		{
			CLI_PRINTLN("		Voc� PERDEU...");
		}
		CLI_PRINTLN("==========================================");
		CLI_PRINTLN("		Pressione Enter para come�ar um novo jogo:");
	} break;
	default: break;
	}
}

void ui_poll_input(Game* game)
{
	fflush(stdout);
	switch (game->state)
	{
	case GameState::STARTING:
	{
		int input_difficulty = 0;
		while (input_difficulty <= 0 || input_difficulty > 3)
		{
			scanf(" %d", &input_difficulty);
		}
		game->start((GameDifficulty)input_difficulty);
	} break;
	case GameState::PLAYING:
	{
		int x = -1;
		int y = -1;
		while (!game->board.is_on_bounds(x, y))
		{
			scanf(" %d %d", &x, &y);
		}
		game->process_action(GameAction::REVEAL_TILE, x, y);
	} break;
	case GameState::ENDED:
	{
		getchar();
		getchar();
		game->reset();
	} break;
	default: break;
	}
}