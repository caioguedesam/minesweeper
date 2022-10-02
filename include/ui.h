#pragma once
#include "game.h"

#include <SFML/Graphics.hpp>

/*
	TODO: Immediate mode GUI version
*/

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

struct UIButtonInfo
{
	bool interactable = true;
	sf::Color color_standard;
	sf::Color color_hovered;
	sf::Color color_pressed;
	
	bool use_outline = false;
	float outline_thickness = 0.f;
	sf::Color color_outline_standard;
	sf::Color color_outline_hovered;
	sf::Color color_outline_pressed;
};

struct UITextInfo
{
	sf::Font* font = nullptr;
	sf::Color color;
	int pixel_size = 0;
	sf::Text::Style style;
};

void ui_text(const char* text_str, sf::Vector2f text_pos, UITextInfo text_info, bool text_centered = false);
bool ui_button(sf::Vector2f button_pos, sf::Vector2f button_size, UIButtonInfo button_info);
bool ui_button(sf::Vector2f button_pos, sf::Vector2f button_size, UIButtonInfo button_info, const char* text_str,
	sf::Vector2f text_relative_pos, UITextInfo text_info, bool text_centered = false);

struct UIState
{
	sf::RenderWindow window;
	MouseState mouse;
	sf::Color color_background;
};

struct UIResources
{
	sf::Font font_default;
	// TODO: Add default sprites (bomb, flag, etc.)
};

void ui_init();
void ui_update_mouse_button_state(bool new_pressed, MouseButton* button);
void ui_poll_input(Game* game);
void ui_clear();
void ui_render_board(Game* game, bool show_all = false);
void ui_render_game(Game* game);
void ui_present();