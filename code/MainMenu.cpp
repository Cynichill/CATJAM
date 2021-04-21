#pragma once
#include "MainMenu.h"

Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("res/fonts/Arial.ttf"))
	{
		// handle error
	}

	text[0].setFont(font);
	text[0].setColor(sf::Color::Red);
	text[0].setString("Play");
	text[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	text[1].setFont(font);
	text[1].setColor(sf::Color::White);
	text[1].setString("Options");
	text[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	text[2].setFont(font);
	text[2].setColor(sf::Color::White);
	text[2].setString("Exit");
	text[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	selectedItemIndex = 0;
}

Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(text[i]);
	}
}