#include "SFML/Graphics.hpp"
#include <iostream>
#include "Mouse.h"

Mouse::Mouse(float width, float height)
{

	if (!cursor.loadFromFile("res/mouse.png"))
	{
		//Error
	}
	cursorSprite.setTexture(cursor);
	widthStore = width;
	heightStore = height;
	cursorSpeed = 10.0f;
}

Mouse::~Mouse()
{
}

void Mouse::draw(sf::RenderWindow& window)
{
	window.draw(cursorSprite);
}

void Mouse::MoveMouse(sf::RenderWindow& window)
{
	//Sprite following mouse position
	cursorSprite.setPosition(sf::Mouse::getPosition(window).x - 7.0f, sf::Mouse::getPosition(window).y);
}

void Mouse::MoveKey()
{

	std::cout << cursorSprite.getPosition().x << " " << cursorSprite.getPosition().y << std::endl;
	//Move cursor Down
	if (cursorSprite.getPosition().y <= heightStore - 16.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		cursorSprite.move(sf::Vector2f(0, cursorSpeed));
	}

	//Move cursor Up
	if (cursorSprite.getPosition().y >= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		cursorSprite.move(sf::Vector2f(0, -cursorSpeed));
	}

	//Move cursor Right
	if (cursorSprite.getPosition().x <= heightStore - 16.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		cursorSprite.move(sf::Vector2f(cursorSpeed, 0));
	}

	//Move cursor Left
	if (cursorSprite.getPosition().x >= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		cursorSprite.move(sf::Vector2f(-cursorSpeed, 0));
	}
}

sf::Vector2f Mouse::getPosition()
{
	return cursorSprite.getPosition();
}