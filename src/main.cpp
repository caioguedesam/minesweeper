///*
//
//Main loop w/ CLI:
//	> UI clear
//	> UI render game state
//		> state dependant
//	> UI polls for user input (waits on scanf)
//		> when input is read, process input for given game state
//
//TODO: main loop w/ GUI
//*/
//
//#include "ui.h"
//
//int main()
//{
//	Game game;
//
//	while (true)
//	{
//		ui_clear();
//		ui_render_game(&game);
//		ui_poll_input(&game);
//		game.check_end_condition();
//	}
//
//	return 0;
//}

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}