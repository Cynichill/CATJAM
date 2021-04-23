#include "SFML/Graphics.hpp"
#include <iostream>
#include "MainMenu.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML WORK!");
	window.setFramerateLimit(60);
	Menu menu(window.getSize().x, window.getSize().y);


	while (window.isOpen())
	{

		window.clear();


		menu.draw(window);
		menu.MouseCheck(window);
		menu.MenuControls(window);

		window.display();
	}
}