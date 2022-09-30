#pragma once
#include "ui.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APP_NAME    "Campo Minado!"
#define APP_DEFAULT_WIDTH       640
#define APP_DEFAULT_HEIGHT      640

#define CLI_PRINT(MSG, ...)\
do {\
	printf(MSG, __VA_ARGS__);\
} while(false)

#define CLI_PRINTLN(MSG, ...)\
do {\
	printf(MSG, __VA_ARGS__);\
	printf("\n");\
} while(false)

UIState ui_state;
UIButtonColors ui_colors_button_standard =
{
	sf::Color(189, 189, 189, 255),
	sf::Color(150, 150, 150, 255),
	sf::Color(82, 82, 82, 255),
};

bool is_on_rectangle(sf::Vector2f point, const sf::RectangleShape & rectangle)
{
	sf::Vector2f rectangle_pos = rectangle.getPosition();
	sf::Vector2f rectangle_size = rectangle.getSize();
	sf::Vector2f rectangle_min =
	{
		rectangle_pos.x,
		rectangle_pos.y,
	};
	sf::Vector2f rectangle_max =
	{
		rectangle_pos.x + rectangle_size.x,
		rectangle_pos.y + rectangle_size.y,
	};
	return point.x >= rectangle_min.x && point.x <= rectangle_max.x
		&& point.y >= rectangle_min.y && point.y <= rectangle_max.y;
}

bool ui_button(sf::Vector2f button_pos, sf::Vector2f button_size, UIButtonColors button_colors)
{
	UIButton button;
	sf::Vector2f button_center_pos = button_pos;
	button.rect.setPosition(button_center_pos);
	button.rect.setSize(button_size);
	button.rect.setFillColor(button_colors.standard);

	bool result = false;
	if (is_on_rectangle(sf::Vector2f(ui_state.mouse.pos_x, ui_state.mouse.pos_y), button.rect))
	{
		button.rect.setFillColor(button_colors.hovered);
		if (ui_state.mouse.is_pressed)
		{
			button.rect.setFillColor(button_colors.pressed);
		}
		if (ui_state.mouse.is_up)
		{
			result = true;
		}
	}

	ui_state.window.draw(button.rect);
	return result;
}

void ui_init()
{
	ui_state.window.create(sf::VideoMode(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT), APP_NAME);
	ui_state.color_background = sf::Color(255, 255, 255, 255);
}

void ui_poll_input(Game* game)
{
	////fflush(stdout);
	//switch (game->state)
	//{
	//case GameState::STARTING:
	//{
	//	int input_difficulty = 0;
	//	while (input_difficulty <= 0 || input_difficulty > 3)
	//	{
	//		scanf(" %d", &input_difficulty);
	//	}
	//	game->start((GameDifficulty)input_difficulty);
	//} break;
	//case GameState::PLAYING:
	//{
	//	int x = -1;
	//	int y = -1;
	//	while (!game->board.is_on_bounds(x, y))
	//	{
	//		scanf(" %d %d", &x, &y);
	//	}
	//	game->process_action(GameAction::REVEAL_TILE, x, y);
	//} break;
	//case GameState::ENDED:
	//{
	//	getchar();
	//	getchar();
	//	game->reset();
	//} break;
	//default: break;
	//}

	// Mouse data
	MouseState& mouse = ui_state.mouse;
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(ui_state.window);
	mouse.pos_x = mouse_pos.x;
	mouse.pos_y = mouse_pos.y;
	bool mouse_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	
	if (mouse.is_down) mouse.is_down = false;
	else if(!mouse.is_pressed && mouse_pressed)
	{
		mouse.is_down = true;
	}
	if (mouse.is_up) mouse.is_up = false;
	else if (mouse.is_pressed && !mouse_pressed)
	{
		mouse.is_up = true;
	}
	mouse.is_pressed = mouse_pressed;

	// Window events
	sf::Window& window = ui_state.window;
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			game->is_running = false;
			break;
		default:
			break;
		}
	}
}

void ui_clear()
{
	//system("cls || clear");		// Clear on linux and windows hack :p
	ui_state.window.clear(ui_state.color_background);
}

void ui_render_board(Game* game, bool show_all)
{
	Board& board = game->board;

	CLI_PRINT("%*c", 5, ' ');

	for (int j = 0; j < board.width; j++)
	{
		CLI_PRINT("%*d", 5, j);
	}
	CLI_PRINTLN("");
	for (int j = 0; j <= board.height; j++)
	{
		CLI_PRINT("%*c", 5, '-');
	}
	CLI_PRINTLN("");

	for (int i = 0; i < board.height; i++)
	{
		CLI_PRINT("%*d|", 4, i);
		for (int j = 0; j < board.width; j++)
		{
			Tile& tile = board.grid[i][j];
			char visual = '.';
			if (tile.revealed || show_all)
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
		// CLI
		/*CLI_PRINTLN("============== Campo Minado ==============");
		CLI_PRINTLN("========= Selecionar Dificuldade =========");
		CLI_PRINTLN("		1. Iniciante (9x9 - 10 bombas)");
		CLI_PRINTLN("		2. Intermediario (16x16 - 40 bombas)");
		CLI_PRINTLN("		3. Expert (16x30 - 99 bombas)");
		CLI_PRINTLN("==========================================");
		CLI_PRINTLN("		Digite o numero da dificuldade desejada:");*/

		// GUI
		ui_button(sf::Vector2f(0, 0), sf::Vector2f(100, 100), ui_colors_button_standard);
		
		
		
		/*sf::CircleShape debug_mouse;
		debug_mouse.setPosition(sf::Vector2f(ui_state.mouse.pos_x, ui_state.mouse.pos_y));
		debug_mouse.setFillColor(sf::Color(255, 255, 0, 255));
		debug_mouse.setRadius(3.f);
		ui_state.window.draw(debug_mouse);*/
	} break;
	case GameState::PLAYING:
	{
		CLI_PRINTLN("==========================================");
		ui_render_board(game);
		CLI_PRINTLN("==========================================");
		CLI_PRINTLN("		Proxima posicao:");
	} break;
	case GameState::ENDED:
	{
		CLI_PRINTLN("============== FIM DE JOGO! ==============");
		if (game->is_won())
		{
			CLI_PRINTLN("		Você GANHOU!");
		}
		else
		{
			CLI_PRINTLN("		Você PERDEU...");
		}
		CLI_PRINTLN("==========================================");
		ui_render_board(game, true);
		CLI_PRINTLN("==========================================");
		CLI_PRINTLN("		Pressione Enter para comecar um novo jogo:");
	} break;
	default: break;
	}
}

void ui_present()
{
	ui_state.window.display();
}