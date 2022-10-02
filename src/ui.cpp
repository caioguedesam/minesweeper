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

// ===========================================
// Estado global e configurações padrão
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
	sf::Color(75, 75, 75, 255),
	sf::Color(75, 75, 75, 255),
	sf::Color(75, 75, 75, 255),
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
// ===========================================
// Funções de UI auxiliares
bool is_on_rectangle(sf::Vector2f point, const sf::RectangleShape& rectangle)
{
	return rectangle.getGlobalBounds().contains(point);
}

void ui_text(const UITextInfo& text_info, const sf::Vector2f& text_pos)
{
	ASSERT(text_info.font, "[UI:TEXT] Fonte invalida para renderizar texto.");
	sf::Text text;
	text.setFont(*text_info.font);
	text.setFillColor(text_info.color);
	text.setCharacterSize(text_info.pixel_size);
	text.setStyle(text_info.style);
	text.setString(text_info.text_str);
	sf::Vector2f pos = text_pos;
	if (text_info.centered)
	{
		sf::FloatRect text_bounds = text.getGlobalBounds();
		pos -= sf::Vector2f(text_bounds.width / 2.f, text_bounds.height / 2.f);
		pos.y -= text_info.pixel_size / 4.f;
	}
	text.setPosition(pos);

	ui_state.window.draw(text);
}

void ui_sprite(const UISpriteInfo& sprite_info, const sf::Vector2f& sprite_pos)
{
	ASSERT(sprite_info.texture, "[UI:SPRITE] Textura invalida para renderizar sprite.");
	sf::Sprite sprite;
	sprite.setTexture(*sprite_info.texture);
	sprite.setColor(sprite_info.color);
	sprite.setScale(sprite_info.scale);
	sf::Vector2f pos = sprite_pos;
	if (sprite_info.centered)
	{
		sf::FloatRect sprite_rect = sprite.getGlobalBounds();
		pos.x -= sprite_rect.width / 2.f;
		pos.y -= sprite_rect.height / 2.f;
	}
	sprite.setPosition(pos);

	ui_state.window.draw(sprite);
}

bool ui_button(const UIButtonInfo& button_info, const sf::Vector2f& button_pos, const sf::Vector2f& button_size)
{
	sf::RectangleShape button_rect;
	sf::Vector2f button_center_pos = button_pos;
	button_rect.setPosition(button_center_pos);
	button_rect.setSize(button_size);
	button_rect.setFillColor(button_info.color_standard);
	button_rect.setOutlineThickness(button_info.has_outline ? button_info.outline_thickness : 0.f);
	button_rect.setOutlineColor(button_info.color_outline_standard);

	// Interação com botão ao clicar com mouse esquerdo/direito
	bool result = false;
	if (button_info.is_interactable && is_on_rectangle(sf::Vector2f(ui_state.mouse.pos_x, ui_state.mouse.pos_y), button_rect))
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

	if (button_info.has_text)
	{
		ui_text(button_info.text_info, button_pos + (button_size / 2.f));
	}

	if (button_info.has_sprite)
	{
		ui_sprite(button_info.sprite_info, button_pos + (button_size / 2.f));
	}

	return result;
}
// ===========================================
// Funções de UI do jogo
void ui_init()
{
	// Inicializando janela
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	ui_state.window.create(sf::VideoMode(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close, settings);
	ui_state.color_background = sf::Color(255, 255, 255, 255);

	// Carregando recursos do jogo
	bool result = ui_resources.font_default.loadFromFile("resources/fonts/Kenney Future Narrow.ttf");
	ASSERT(result, "[UI:RESOURCE] Falha no carregamento da fonte padrão.");
	result = ui_resources.texture_bomb.loadFromFile("resources/textures/mine-explosion.png");
	ASSERT(result, "[UI:RESOURCE] Falha ao carregar textura de mina.");
	result = ui_resources.texture_flag.loadFromFile("resources/textures/flying-flag.png");
	ASSERT(result, "[UI:RESOURCE] Falha ao carregar textura de bandeira.");
	result = ui_resources.texture_radar.loadFromFile("resources/textures/radar-sweep.png");
	ASSERT(result, "[UI:RESOURCE] Falha ao carregar textura de radar.");
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
	tile_text_info.centered = true;
	char tile_text_str[10];

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
			UIButtonInfo button_info = tile_visible ? ui_button_info_tile_revealed : ui_button_info_tile;
			tile_text_info.color = sf::Color(0, 0, 0, 255);

			// Tiles de bomba: a bomba só é visível quando tile é revelado.
			if (tile_visible && tile.has_bomb)
			{
				UISpriteInfo sprite_bomb;
				sprite_bomb.texture = &ui_resources.texture_bomb;
				sprite_bomb.color = sf::Color(255, 100, 0, 255);
				sprite_bomb.scale = (game->difficulty == GameDifficulty::BEGINNER) ? sf::Vector2f(.085f, .085f) : sf::Vector2f(.045f, .045f);
				sprite_bomb.centered = true;
				button_info.has_sprite = true;
				button_info.sprite_info = sprite_bomb;
			}
			else if (tile_visible && tile.has_radar)
			{
				UISpriteInfo sprite_radar;
				sprite_radar.texture = &ui_resources.texture_radar;
				sprite_radar.color = sf::Color(0, 100, 168, 255);
				sprite_radar.scale = (game->difficulty == GameDifficulty::BEGINNER) ? sf::Vector2f(.085f, .085f) : sf::Vector2f(.045f, .045f);
				sprite_radar.centered = true;
				button_info.has_sprite = true;
				button_info.sprite_info = sprite_radar;
			}
			// Tiles de bandeira: bandeira é visível apenas em tiles não revelados e marcados.
			else if (!tile_visible && tile.has_flag)
			{
				UISpriteInfo sprite_flag;
				sprite_flag.texture = &ui_resources.texture_flag;
				sprite_flag.color = sf::Color(168, 0, 0, 255);
				sprite_flag.scale = (game->difficulty == GameDifficulty::BEGINNER) ? sf::Vector2f(.085f, .085f) : sf::Vector2f(.045f, .045f);
				sprite_flag.centered = true;
				button_info.has_sprite = true;
				button_info.sprite_info = sprite_flag;
			}
			else
			{
				button_info.has_sprite = false;
			}
			
			// Tiles comuns: caso estejam revelados, marcam seu número de bombas adjacentes no tile.
			if (tile_visible && !tile.has_bomb && tile.adjacent_bombs > 0)
			{
				tile_text_info.color = tile_indicator_colors[tile.adjacent_bombs - 1];
				sprintf(tile_text_str, "%d", tile.adjacent_bombs);
				tile_text_info.text_str = tile_text_str;
				button_info.has_text = true;
				button_info.text_info = tile_text_info;
			}
			else
			{
				button_info.has_text = false;
			}

			// Interação com o determinado tile.
			if (ui_button(button_info, tile_pos, tile_size))
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
		// Tela inicial do jogo, com nome do jogo e seleção de modo de jogo.
		ui_state.window.setSize(sf::Vector2u(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT));
		sf::FloatRect ui_view_rect = sf::FloatRect(0, 0, APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT);
		ui_state.window.setView(sf::View(ui_view_rect));

		sf::Vector2f panel_position = sf::Vector2f(50, 30);

		UITextInfo text_info;
		text_info.font = &ui_resources.font_default;
		text_info.color = sf::Color(0, 0, 0, 255);
		text_info.pixel_size = 48;
		text_info.style = sf::Text::Style::Bold;
		text_info.text_str = "Campo Minado";
		ui_text(text_info, panel_position);

		panel_position.y += 100;
		text_info.pixel_size = 32;
		text_info.style = sf::Text::Style::Regular;
		text_info.text_str = "Novo jogo:";
		ui_text(text_info, panel_position);

		panel_position.x += 20;
		panel_position.y += 65;
		sf::Vector2f button_size = sf::Vector2f(350, 50);
		text_info.centered = true;
		text_info.text_str = "Iniciante";
		UIButtonInfo button_info = ui_button_info_default;
		button_info.has_text = true;
		button_info.text_info = text_info;
		if (ui_button(button_info, panel_position, button_size))
		{
			game->start(GameDifficulty::BEGINNER);
		}
		panel_position.y += 80;
		text_info.text_str = "Intermediário";
		button_info.text_info = text_info;
		if(ui_button(button_info, panel_position, button_size))
		{
			game->start(GameDifficulty::INTERMEDIATE);
		}
		panel_position.y += 80;
		text_info.text_str = "Expert";
		button_info.text_info = text_info;
		if (ui_button(button_info, panel_position, button_size))
		{
			game->start(GameDifficulty::EXPERT);
			ui_state.window.setSize(sf::Vector2u(APP_EXPANDED_WIDTH, APP_DEFAULT_HEIGHT));
			sf::FloatRect ui_view_rect = sf::FloatRect(0, 0, APP_EXPANDED_WIDTH, APP_DEFAULT_HEIGHT);
			ui_state.window.setView(sf::View(ui_view_rect));
		}

	} break;
	case GameState::PLAYING:
	{
		// Jogo corrente. Renderiza o tabuleiro inteiro.
		ui_render_board(game);
	} break;
	case GameState::ENDED:
	{
		// Jogo após finalizar. Renderiza o tabuleiro inteiro e texto de vitória/derrota.
		ui_render_board(game, true);

		sf::Vector2f panel_position = sf::Vector2f(30, APP_DEFAULT_HEIGHT - 45);
		UITextInfo text_info;
		text_info.font = &ui_resources.font_default;
		text_info.color = game->is_won() ? sf::Color(0, 220, 30, 255) : sf::Color(220, 0, 30, 255);
		text_info.pixel_size = 24;
		text_info.style = sf::Text::Style::Bold;
		text_info.text_str = game->is_won() ? "Você ganhou!" : "Você perdeu...";
		ui_text(text_info, panel_position);

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
		text_info.text_str = "Reiniciar";
		text_info.centered = true;
		UIButtonInfo button_info = ui_button_info_default;
		button_info.has_text = true;
		button_info.text_info = text_info;
		if (ui_button(button_info, panel_position, button_size))
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