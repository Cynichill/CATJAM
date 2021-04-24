#include "SFML/Graphics.hpp"
#include <iostream>
#include "MainMenu.h"
#include "Mouse.h"

int main()
{
	//Window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML WORK!");
	window.setFramerateLimit(60);

	//Game State
	int gameState = 1;

	//Control State
	int controlState = 0;

	Menu menu(window.getSize().x, window.getSize().y);
	Mouse mouse(window.getSize().x, window.getSize().y);

	while (window.isOpen())
	{

		window.clear();

		//Game State is in the menu
		if (gameState == 0)
		{
			/*window.setMouseCursorVisible(true);*/

			menu.draw(window);
			menu.MouseCheck(window);
			menu.MenuControls(window);
		}

		mouse.draw(window);

		if (controlState == 0)
		{
			window.setMouseCursorVisible(false);
			mouse.MoveMouse(window);
		}

		if (controlState == 1)
		{
			mouse.MoveKey();
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		window.display();
	}
}