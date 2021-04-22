#pragma once
#include "MainMenu.h"

Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("res/fonts/Arial.ttf"))
	{
		// handle error
	}

	text[0].setFont(font);
	text[0].setColor(sf::Color::Black);
	text[0].setString("Play");
	text[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));
	hitboxes[0].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 0.9));
	hitboxes[0].setSize(sf::Vector2f(200, 100));
	hitboxes[0].setFillColor(sf::Color::White);

	text[1].setFont(font);
	text[1].setColor(sf::Color::Black);
	text[1].setString("Options");
	text[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));
	hitboxes[1].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 1.9));
	hitboxes[1].setSize(sf::Vector2f(200, 100));
	hitboxes[1].setFillColor(sf::Color::White);

	text[2].setFont(font);
	text[2].setColor(sf::Color::Black);
	text[2].setString("Exit");
	text[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));
	hitboxes[2].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.9));
	hitboxes[2].setSize(sf::Vector2f(200, 100));
	hitboxes[2].setFillColor(sf::Color::White);

	selectedItemIndex = 0;
}

Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(hitboxes[i]);
		window.draw(text[i]);
	}
}

void Menu::MouseCheck(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		if (hitboxes[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		{
			text[i].setColor(sf::Color::Red);
			hitboxes[i].setFillColor(sf::Color::Blue);

			if (i == 2 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				window.close();
			}
		}
		else
		{
			text[i].setColor(sf::Color::Black);
			hitboxes[i].setFillColor(sf::Color::White);
		}
	}
}