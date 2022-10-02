#pragma once
#include "ui.h"
#include "game.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APP_NAME    "Campo Minado!"
#define APP_DEFAULT_WIDTH       480
#define APP_DEFAULT_HEIGHT      480

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
UIButtonInfo ui_button_info_default =
{
	true,
	sf::Color(189, 189, 189, 255),
	sf::Color(150, 150, 150, 255),
	sf::Color(82, 82, 82, 255),
	true,
	3.f,
	sf::Color(150, 150, 150, 255),
	sf::Color(82, 82, 82, 255),
	sf::Color(82, 82, 82, 255),
};
UIButtonInfo ui_button_info_tile =
{
	true,
	sf::Color(189, 189, 189, 255),
	sf::Color(150, 150, 150, 255),
	sf::Color(82, 82, 82, 255),
	true,
	1.f,
	sf::Color(150, 150, 150, 255),
	sf::Color(82, 82, 82, 255),
	sf::Color(82, 82, 82, 255),
};
UIButtonInfo ui_button_info_tile_revealed =
{
	false,
	sf::Color(150, 150, 150, 255),
	sf::Color(150, 150, 150, 255),
	sf::Color(82, 82, 82, 255),
	true,
	1.f,
	sf::Color(150, 150, 150, 255),
	sf::Color(82, 82, 82, 255),
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

bool ui_button(sf::Vector2f button_pos, sf::Vector2f button_size, UIButtonInfo button_info)
{
	sf::RectangleShape button_rect;
	sf::Vector2f button_center_pos = button_pos;
	button_rect.setPosition(button_center_pos);
	button_rect.setSize(button_size);
	button_rect.setFillColor(button_info.color_standard);
	button_rect.setOutlineThickness(button_info.use_outline ? button_info.outline_thickness : 0.f);
	button_rect.setOutlineColor(button_info.color_outline_standard);

	bool result = false;
	if (button_info.interactable && is_on_rectangle(sf::Vector2f(ui_state.mouse.pos_x, ui_state.mouse.pos_y), button_rect))
	{
		button_rect.setFillColor(button_info.color_hovered);
		button_rect.setOutlineColor(button_info.color_outline_hovered);
		if (ui_state.mouse.is_pressed)
		{
			button_rect.setFillColor(button_info.color_pressed);
			button_rect.setOutlineColor(button_info.color_outline_pressed);
		}
		if (ui_state.mouse.is_up)
		{
			result = true;
		}
	}

	ui_state.window.draw(button_rect);
	return result;
}

bool ui_button(sf::Vector2f button_pos, sf::Vector2f button_size, UIButtonInfo button_info, const char* text_str,
	sf::Vector2f text_relative_pos, UITextInfo text_info, bool text_centered)
{
	// TODO_UI: Esses valores de posicionamento centralizado e não centralizado 
	// não me parecem totalmente certos. Olhar depois...
	bool result = ui_button(button_pos, button_size, button_info);
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

	sf::Vector2f board_origin = sf::Vector2f(25, 25);
	sf::Vector2f tile_pos = board_origin;
	sf::Vector2f tile_size;
	UITextInfo tile_text_info;
	tile_text_info.font = &ui_resources.font_default;
	tile_text_info.style = sf::Text::Style::Bold;

	if (game->difficulty == GameDifficulty::BEGINNER)
	{
		tile_size = sf::Vector2f(45, 45);
		tile_text_info.pixel_size = 32;
	}
	else
	{
		tile_size = sf::Vector2f(25, 25);
		tile_text_info.pixel_size = 18;
	}

	for (int x = 0; x < board.height; x++)
	{
		for (int y = 0; y < board.width; y++)
		{
			Tile& tile = board.grid[x][y];
			bool tile_visible = tile.revealed || show_all;
			UIButtonInfo& button_info = tile_visible ? ui_button_info_tile_revealed : ui_button_info_tile;
			tile_text_info.color = sf::Color(0, 0, 0, 255);
			char tile_label[10];
			if (tile_visible && tile.has_bomb)
			{
				sprintf(tile_label, "B");
				tile_text_info.color = sf::Color(255, 0, 0, 255);
			}
			else if (tile_visible && tile.adjacent_bombs > 0)
			{
				sprintf(tile_label, "%d", tile.adjacent_bombs);
			}
			else if (!tile_visible && tile.has_flag)
			{
				sprintf(tile_label, "F");
				tile_text_info.color = sf::Color(0, 0, 255, 255);
			}
			else
			{
				sprintf(tile_label, " ");
			}
			if (ui_button(tile_pos, tile_size, button_info, tile_label, sf::Vector2f(0, 0), tile_text_info, true))
			{
				game->process_action(GameAction::REVEAL_TILE, x, y);
			}

			tile_pos.x += tile_size.x;
		}
		tile_pos.x = board_origin.x;
		tile_pos.y += tile_size.y;
	}
}

void ui_render_game(Game* game)
{
	switch (game->state)
	{
	case GameState::STARTING:
	{
		ui_state.window.setSize(sf::Vector2u(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT));
		sf::FloatRect ui_view_rect = sf::FloatRect(0, 0, APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT);
		ui_state.window.setView(sf::View(ui_view_rect));

		sf::Vector2f panel_position = sf::Vector2f(50, 30);

		UITextInfo text_info;
		text_info.font = &ui_resources.font_default;
		text_info.color = sf::Color(0, 0, 0, 255);
		text_info.pixel_size = 48;
		text_info.style = sf::Text::Style::Bold;
		ui_text("Campo Minado", panel_position, text_info);

		panel_position.y += 100;
		text_info.pixel_size = 32;
		text_info.style = sf::Text::Style::Regular;
		ui_text("Novo jogo:", panel_position, text_info);

		panel_position.x += 20;
		panel_position.y += 65;
		sf::Vector2f button_size = sf::Vector2f(350, 50);
		if (ui_button(panel_position, button_size, ui_button_info_default, "Iniciante", sf::Vector2f(0, 0), text_info, true))
		{
			game->start(GameDifficulty::BEGINNER);
		}
		panel_position.y += 80;
		if(ui_button(panel_position, button_size, ui_button_info_default, "Intermediário", sf::Vector2f(0, 0), text_info, true))
		{
			game->start(GameDifficulty::INTERMEDIATE);
		}
		panel_position.y += 80;
		if (ui_button(panel_position, button_size, ui_button_info_default, "Expert", sf::Vector2f(0, 0), text_info, true))
		{
			game->start(GameDifficulty::EXPERT);
			ui_state.window.setSize(sf::Vector2u(APP_DEFAULT_WIDTH * 1.7f, APP_DEFAULT_HEIGHT));
			sf::FloatRect ui_view_rect = sf::FloatRect(0, 0, APP_DEFAULT_WIDTH * 1.7f, APP_DEFAULT_HEIGHT);
			ui_state.window.setView(sf::View(ui_view_rect));
		}

	} break;
	case GameState::PLAYING:
	{
		ui_render_board(game);
	} break;
	case GameState::ENDED:
	{
		//CLI_PRINTLN("============== FIM DE JOGO! ==============");
		//if (game->is_won())
		//{
		//	CLI_PRINTLN("		Você GANHOU!");
		//}
		//else
		//{
		//	CLI_PRINTLN("		Você PERDEU...");
		//}
		//CLI_PRINTLN("==========================================");
		ui_render_board(game, true);
		//CLI_PRINTLN("==========================================");
		//CLI_PRINTLN("		Pressione Enter para comecar um novo jogo:");
	} break;
	default: break;
	}
}

void ui_present()
{
	ui_state.window.display();
}