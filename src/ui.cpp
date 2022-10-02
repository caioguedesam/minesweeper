#pragma once
#include "ui.h"
#include "game.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APP_NAME    "Campo Minado!"
#define APP_DEFAULT_WIDTH       480
#define APP_EXPANDED_WIDTH      800
#define APP_DEFAULT_HEIGHT      480

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
	false,
};

sf::Color tile_indicator_colors[] =
{
	sf::Color(17, 20, 94, 255),		// 1
	sf::Color(30, 128, 28, 255),	// 2
	sf::Color(156, 10, 8, 255),		// 3
	sf::Color(50, 14, 105, 255),	// 4
	sf::Color(51, 27, 18, 255),		// 5
	sf::Color(42, 94, 74, 255),		// 6
	sf::Color(0, 0, 0, 255),		// 7
	sf::Color(59, 59, 59, 255),		// 8
};

bool is_on_rectangle(sf::Vector2f point, const sf::RectangleShape& rectangle)
{
	return rectangle.getGlobalBounds().contains(point);
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
		if (ui_state.mouse.button_left.is_pressed)
		{
			button_rect.setFillColor(button_info.color_pressed);
			button_rect.setOutlineColor(button_info.color_outline_pressed);
		}
		if (ui_state.mouse.button_left.is_up || ui_state.mouse.button_right.is_up)
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
	// Inicializando janela
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	ui_state.window.create(sf::VideoMode(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close, settings);
	ui_state.color_background = sf::Color(255, 255, 255, 255);

	// Carregando recursos do jogo
	bool result = ui_resources.font_default.loadFromFile("resources/fonts/Kenney Future Narrow.ttf");
	ASSERT(result, "[UI:RESOURCE] Failed to load default font.");
	result = ui_resources.texture_bomb.loadFromFile("resources/textures/skull.png");
	ASSERT(result, "[UI:RESOURCE] Failed to load bomb texture.");
	result = ui_resources.texture_flag.loadFromFile("resources/textures/flag.png");
	ASSERT(result, "[UI:RESOURCE] Failed to load flag texture.");
}

void ui_update_mouse_button_state(bool new_pressed, MouseButton* button)
{
	if (button->is_down) button->is_down = false;
	else if (!button->is_pressed && new_pressed)
	{
		button->is_down = true;
	}

	if (button->is_up) button->is_up = false;
	else if (button->is_pressed && !new_pressed)
	{
		button->is_up = true;
	}

	button->is_pressed = new_pressed;
}

void ui_poll_input(Game* game)
{
	// Estado atual do mouse
	MouseState& mouse = ui_state.mouse;
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(ui_state.window);
	mouse.pos_x = mouse_pos.x;
	mouse.pos_y = mouse_pos.y;
	ui_update_mouse_button_state(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left), &mouse.button_left);
	ui_update_mouse_button_state(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right), &mouse.button_right);

	// Eventos de janela
	sf::Window& window = ui_state.window;
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			game->exit();
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
			if (tile_visible && !tile.has_bomb && tile.adjacent_bombs > 0)
			{
				tile_text_info.color = tile_indicator_colors[tile.adjacent_bombs - 1];
				sprintf(tile_label, "%d", tile.adjacent_bombs);
			}
			else
			{
				sprintf(tile_label, " ");
			}
			if (ui_button(tile_pos, tile_size, button_info, tile_label, sf::Vector2f(0, 0), tile_text_info, true))
			{
				if (ui_state.mouse.button_left.is_up)
				{
					game->action_reveal_tile(x, y);
				}
				else
				{
					game->action_toggle_flag(x, y);
				}
			}
			if (tile_visible && tile.has_bomb)
			{
				sf::Sprite sprite_bomb;
				sprite_bomb.setTexture(ui_resources.texture_bomb);
				sprite_bomb.setColor(sf::Color(0, 0, 0, 255));
				sprite_bomb.setScale(game->difficulty != GameDifficulty::EXPERT ? sf::Vector2f(.5f, .5f) : sf::Vector2f(.25f, .25f));
				sf::Vector2f sprite_pos = tile_pos;
				sf::FloatRect sprite_rect = sprite_bomb.getGlobalBounds();
				sprite_pos.x -= sprite_rect.width / 2.f;
				sprite_pos.y -= sprite_rect.height / 2.f;
				sprite_pos += tile_size / 2.f;
				sprite_bomb.setPosition(sprite_pos);
				ui_state.window.draw(sprite_bomb);
			}
			else if (!tile_visible && tile.has_flag)
			{
				sf::Sprite sprite_flag;
				sprite_flag.setTexture(ui_resources.texture_flag);
				sprite_flag.setColor(sf::Color(210, 0, 0, 255));
				sprite_flag.setScale(game->difficulty != GameDifficulty::EXPERT ? sf::Vector2f(.5f, .5f) : sf::Vector2f(.25f, .25f));
				sf::Vector2f sprite_pos = tile_pos;
				sf::FloatRect sprite_rect = sprite_flag.getGlobalBounds();
				sprite_pos.x -= sprite_rect.width / 2.f;
				sprite_pos.y -= sprite_rect.height / 2.f;
				sprite_pos += tile_size / 2.f;
				sprite_flag.setPosition(sprite_pos);
				ui_state.window.draw(sprite_flag);
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
			ui_state.window.setSize(sf::Vector2u(APP_EXPANDED_WIDTH, APP_DEFAULT_HEIGHT));
			sf::FloatRect ui_view_rect = sf::FloatRect(0, 0, APP_EXPANDED_WIDTH, APP_DEFAULT_HEIGHT);
			ui_state.window.setView(sf::View(ui_view_rect));
		}

	} break;
	case GameState::PLAYING:
	{
		ui_render_board(game);
	} break;
	case GameState::ENDED:
	{
		ui_render_board(game, true);

		sf::Vector2f panel_position = sf::Vector2f(30, APP_DEFAULT_HEIGHT - 45);
		UITextInfo text_info;
		text_info.font = &ui_resources.font_default;
		text_info.color = game->is_won() ? sf::Color(0, 220, 30, 255) : sf::Color(220, 0, 30, 255);
		text_info.pixel_size = 24;
		text_info.style = sf::Text::Style::Bold;
		ui_text(game->is_won() ? "Você ganhou!" : "Você perdeu...", panel_position, text_info);

		if (game->difficulty == GameDifficulty::EXPERT)
		{
			panel_position.x = APP_EXPANDED_WIDTH;
		}
		else
		{
			panel_position.x = APP_DEFAULT_WIDTH;
		}

		sf::Vector2f button_size = sf::Vector2f(150, 30);
		panel_position.x -= button_size.x + 20;
		panel_position.y += 5;
		text_info.color = sf::Color(0, 0, 0, 255);
		if (ui_button(panel_position, button_size, ui_button_info_default,
			"Reiniciar", sf::Vector2f(0, 0), text_info, true))
		{
			game->reset();
		}

	} break;
	default: break;
	}
}

void ui_present()
{
	ui_state.window.display();
}