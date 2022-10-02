#pragma once
#include "game.h"
#include <SFML/Graphics.hpp>

struct MouseButton
{
	bool is_down = false;
	bool is_pressed = false;
	bool is_up = false;
};

struct MouseState
{
	int pos_x		= 0;
	int pos_y		= 0;
	MouseButton button_left;
	MouseButton button_right;
};

bool is_on_rectangle(sf::Vector2f point, const sf::RectangleShape& rectangle);

struct UITextInfo
{
	const char* text_str = nullptr;
	sf::Font* font = nullptr;
	sf::Color color = sf::Color(0, 0, 0, 255);
	sf::Text::Style style = sf::Text::Style::Regular;
	int pixel_size = 0;
	bool centered = false;
};

struct UISpriteInfo
{
	sf::Texture* texture = nullptr;
	sf::Color color;
	sf::Vector2f scale = sf::Vector2f(1, 1);
	bool centered = false;
};

struct UIButtonInfo
{
	bool is_interactable = true;
	sf::Color color_standard;
	sf::Color color_hovered;
	sf::Color color_pressed;
	
	bool has_outline = false;
	float outline_thickness = 0.f;
	sf::Color color_outline_standard;
	sf::Color color_outline_hovered;
	sf::Color color_outline_pressed;

	bool has_text = false;
	UITextInfo text_info;

	bool has_sprite = false;
	UISpriteInfo sprite_info;
};

void ui_text(const UITextInfo& text_info, const sf::Vector2f& text_pos);
void ui_sprite(const UISpriteInfo& sprite_info, const sf::Vector2f& sprite_pos);
bool ui_button(const UIButtonInfo& button_info, const sf::Vector2f& button_pos, const sf::Vector2f& button_size);

struct UIState
{
	sf::RenderWindow window;
	MouseState mouse;
	sf::Color color_background;
};

struct UIResources
{
	sf::Font font_default;
	sf::Texture texture_bomb;
	sf::Texture texture_flag;
	sf::Texture texture_radar;
};

void ui_init();
void ui_update_mouse_button_state(bool new_pressed, MouseButton* button);
void ui_poll_input(Game* game);
void ui_clear();
void ui_render_board(Game* game, bool show_all = false);
void ui_render_game(Game* game);
void ui_present();