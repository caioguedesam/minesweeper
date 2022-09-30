#pragma once
#include "ui.h"
#include "game.h"
#include "assert.h"
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
UIResources ui_resources;
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

void ui_text(const char* text_str, sf::Vector2f text_pos, UITextInfo text_info, bool text_centered)
{
	ASSERT(text_info.font, "[UI:TEXT] Invalid font for rendering text");
	sf::Text text;
	text.setFont(*text_info.font);
	text.setFillColor(text_info.color);
	text.setCharacterSize(text_info.pixel_size);
	text.setStyle(text_info.style);
	text.setString(text_str);

	if (text_centered)
	{
		sf::FloatRect text_bounds = text.getGlobalBounds();
		sf::Vector2f center_offset = sf::Vector2f(text_bounds.width / 2.f, text_bounds.height / 2.f);
		text.setPosition(text_pos - center_offset);
	}
	else
	{
		text.setPosition(text_pos);
	}

	ui_state.window.draw(text);
}

bool ui_button(sf::Vector2f button_pos, sf::Vector2f button_size, UIButtonColors button_colors)
{
	sf::RectangleShape button_rect;
	sf::Vector2f button_center_pos = button_pos;
	button_rect.setPosition(button_center_pos);
	button_rect.setSize(button_size);
	button_rect.setFillColor(button_colors.standard);

	bool result = false;
	if (is_on_rectangle(sf::Vector2f(ui_state.mouse.pos_x, ui_state.mouse.pos_y), button_rect))
	{
		button_rect.setFillColor(button_colors.hovered);
		if (ui_state.mouse.is_pressed)
		{
			button_rect.setFillColor(button_colors.pressed);
		}
		if (ui_state.mouse.is_up)
		{
			result = true;
		}
	}

	ui_state.window.draw(button_rect);
	return result;
}

bool ui_button(sf::Vector2f button_pos, sf::Vector2f button_size, UIButtonColors button_colors, const char* text_str,
	sf::Vector2f text_relative_pos, UITextInfo text_info, bool text_centered)
{
	// TODO_UI: Esses valores de posicionamento centralizado e não centralizado 
	// não me parecem totalmente certos. Olhar depois...
	bool result = ui_button(button_pos, button_size, button_colors);
	sf::Vector2f text_pos = button_pos;
	if (text_centered)
	{
		text_pos.x += button_size.x / 2.f;
		text_pos.y += (button_size.y / 2.f) - (text_info.pixel_size / 4.f);
	}
	ui_text(text_str, text_pos, text_info, text_centered);
	return result;
}

void ui_init()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	ui_state.window.create(sf::VideoMode(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close, settings);
	ui_state.color_background = sf::Color(255, 255, 255, 255);

	// Load default resources
	const char* default_font_path = "resources/fonts/Kenney Future Narrow.ttf";
	bool result = ui_resources.font_default.loadFromFile(default_font_path);
	ASSERT_FORMAT(result, "[UI:RESOURCE] Failed to load font from file %s", default_font_path);
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
		UITextInfo text_info;
		text_info.font = &ui_resources.font_default;
		text_info.color = sf::Color(0, 0, 0, 255);
		text_info.pixel_size = 48;
		text_info.style = sf::Text::Style::Bold;
		ui_text("Campo Minado", sf::Vector2f(APP_DEFAULT_WIDTH / 2.f, 30), text_info, true);

		sf::Vector2f panel_position = sf::Vector2f(30, 100);
		text_info.pixel_size = 32;
		text_info.style = sf::Text::Style::Regular;
		ui_text("Novo jogo:", panel_position, text_info);

		panel_position.x += 20;
		panel_position.y += 50;
		sf::Vector2f button_size = sf::Vector2f(350, 50);
		ui_button(panel_position, button_size, ui_colors_button_standard, "Iniciante", sf::Vector2f(0, 0), text_info, true);
		panel_position.y += 75;
		ui_button(panel_position, button_size, ui_colors_button_standard, "Intermediário", sf::Vector2f(0, 0), text_info, true);
		panel_position.y += 75;
		ui_button(panel_position, button_size, ui_colors_button_standard, "Expert", sf::Vector2f(0, 0), text_info, true);

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