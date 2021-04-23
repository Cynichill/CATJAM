#pragma once
#include <iostream>
#include "MainMenu.h"

Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("res/fonts/Arial.ttf"))
	{
		// handle error
	}

	text[0].setFont(font);
	text[0].setColor(sf::Color::Red);
	text[0].setString("New Game");
	text[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));
	hitboxes[0].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 0.9));
	hitboxes[0].setSize(sf::Vector2f(250, 100));
	hitboxes[0].setFillColor(sf::Color::Blue);

	text[1].setFont(font);
	text[1].setColor(sf::Color::Black);
	text[1].setString("Load Game");
	text[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));
	hitboxes[1].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 1.9));
	hitboxes[1].setSize(sf::Vector2f(250, 100));
	hitboxes[1].setFillColor(sf::Color::White);

	text[2].setFont(font);
	text[2].setColor(sf::Color::Black);
	text[2].setString("Options");
	text[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));
	hitboxes[2].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.9));
	hitboxes[2].setSize(sf::Vector2f(250, 100));
	hitboxes[2].setFillColor(sf::Color::White);

	text[3].setFont(font);
	text[3].setColor(sf::Color::Black);
	text[3].setString("Exit");
	text[3].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 4));
	hitboxes[3].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.9));
	hitboxes[3].setSize(sf::Vector2f(250, 100));
	hitboxes[3].setFillColor(sf::Color::White);

	selectedItemIndex = 0;

	storeWidth = width;
	storeHeight = height;
}

Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(hitboxes[i]);
		window.draw(text[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		text[selectedItemIndex].setColor(sf::Color::Black);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		text[selectedItemIndex].setColor(sf::Color::Red);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::Blue);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		text[selectedItemIndex].setColor(sf::Color::Black);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		text[selectedItemIndex].setColor(sf::Color::Red);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::Blue);
	}
}

void Menu::MouseCheck(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		checkMouse[i] = false;
		if (hitboxes[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		{
			checkMouse[i] = true;
			text[i].setColor(sf::Color::Red);
			hitboxes[i].setFillColor(sf::Color::Blue);

			for (int j = 0; j < MAX_NUMBER_OF_ITEMS; j++)
			{
				if (j != i)
				{
					text[j].setColor(sf::Color::Black);
					hitboxes[j].setFillColor(sf::Color::White);
				}
			}

			selectedItemIndex = i;

		}
	}
}

void Menu::MenuControls(sf::RenderWindow& window)
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				MoveUp();
				break;

			case sf::Keyboard::Down:
				MoveDown();
				break;

			case sf::Keyboard::Enter:

				switch (GetPressedItem())
				{
				case 0:
					std::cout << "Play button has been pressed" << std::endl;
					break;
				case 1:
					std::cout << "Option button has been pressed" << std::endl;
					break;
				case 2:
					window.close();
					break;
				}

				break;
			}

		case sf::Event::MouseButtonPressed:

			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:

				switch (GetPressedItem())
				{
				case 0:
					if(checkMouse[0])
					std::cout << "New game button has been pressed" << std::endl;
					break;
				case 1:
					if (checkMouse[1])
					std::cout << "Load game button has been pressed" << std::endl;
					break;
				case 2:
					if (checkMouse[2])
					{
						std::cout << "Option button has been pressed" << std::endl;
						menuCheck = 1;
						MenuTransition(storeWidth, storeHeight);
					}
					break;
				case 3:
					if(checkMouse[3])
					window.close();
					break;
				}

				break;
			}


			break;
		case sf::Event::Closed:
			window.close();

			break;

		}
	}
}

void Menu::MenuTransition(float width, float height)
{
	if (menuCheck == 0)
	{
		text[0].setFont(font);
		text[0].setColor(sf::Color::Red);
		text[0].setString("New Game");
		text[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));
		hitboxes[0].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 0.9));
		hitboxes[0].setSize(sf::Vector2f(250, 100));
		hitboxes[0].setFillColor(sf::Color::Blue);

		text[1].setFont(font);
		text[1].setColor(sf::Color::Black);
		text[1].setString("Load Game");
		text[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));
		hitboxes[1].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 1.9));
		hitboxes[1].setSize(sf::Vector2f(250, 100));
		hitboxes[1].setFillColor(sf::Color::White);

		text[2].setFont(font);
		text[2].setColor(sf::Color::Black);
		text[2].setString("Options");
		text[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));
		hitboxes[2].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.9));
		hitboxes[2].setSize(sf::Vector2f(250, 100));
		hitboxes[2].setFillColor(sf::Color::White);

		text[3].setFont(font);
		text[3].setColor(sf::Color::Black);
		text[3].setString("Exit");
		text[3].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 4));
		hitboxes[3].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.9));
		hitboxes[3].setSize(sf::Vector2f(250, 100));
		hitboxes[3].setFillColor(sf::Color::White);
	}

	if (menuCheck == 1)
	{
		text[0].setFont(font);
		text[0].setColor(sf::Color::Red);
		text[0].setString("Sound");
		text[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));
		hitboxes[0].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 0.9));
		hitboxes[0].setSize(sf::Vector2f(250, 100));
		hitboxes[0].setFillColor(sf::Color::Blue);

		text[1].setFont(font);
		text[1].setColor(sf::Color::Black);
		text[1].setString("Screen");
		text[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));
		hitboxes[1].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 1.9));
		hitboxes[1].setSize(sf::Vector2f(250, 100));
		hitboxes[1].setFillColor(sf::Color::White);

		text[2].setFont(font);
		text[2].setColor(sf::Color::Black);
		text[2].setString("Controls");
		text[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));
		hitboxes[2].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.9));
		hitboxes[2].setSize(sf::Vector2f(250, 100));
		hitboxes[2].setFillColor(sf::Color::White);

		text[3].setFont(font);
		text[3].setColor(sf::Color::Black);
		text[3].setString("Back");
		text[3].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 4));
		hitboxes[3].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.9));
		hitboxes[3].setSize(sf::Vector2f(250, 100));
		hitboxes[3].setFillColor(sf::Color::White);
	}
}