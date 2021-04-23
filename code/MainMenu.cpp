#pragma once
#include <iostream>
#include "MainMenu.h"

//What is initilised when we call Menu
Menu::Menu(float width, float height)
{
	//Loads the font required
	if (!font.loadFromFile("res/fonts/Arial.ttf"))
	{
		// handle error
	}

	//Sets the text + hitboxes for the main menu
	//We have an array of text and hitboxes which contain each item in the menu

	text[0].setFont(font); //Sets font
	text[0].setColor(sf::Color::Red); //Sets colour of text
	text[0].setString("New Game"); //Sets Name of text 
	text[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1)); //Sets position of the text based on resolution
	hitboxes[0].setPosition(sf::Vector2f(width / 2.1, height / (MAX_NUMBER_OF_ITEMS + 1) * 0.9)); //Sets position of the hitboxes based on resolution
	hitboxes[0].setSize(sf::Vector2f(250, 100)); //Sets size of the hitboxes
	hitboxes[0].setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes

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

	selectedItemIndex = 0; //Sets the item selected to the first one
	menuCheck = 0; //Sets the menu to the main menu
	maxDraw = 4; //Max Things to draw

	//Stores width and height of the screen
	storeWidth = width; 
	storeHeight = height;
}

Menu::~Menu()
{
}

//Draws each item
void Menu::draw(sf::RenderWindow &window)
{
	for (int i = 0; i < maxDraw; i++)
	{
		window.draw(hitboxes[i]);
		window.draw(text[i]);
	}
}

//Used to move up when using keyboard or controller
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

	//Loops back around
	else if (selectedItemIndex == 0)
	{
		text[selectedItemIndex].setColor(sf::Color::Black);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex = maxDraw - 1;
		text[selectedItemIndex].setColor(sf::Color::Red);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::Blue);
	}
}

//Used to move down when using keyboard or controller
void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < maxDraw)
	{
		text[selectedItemIndex].setColor(sf::Color::Black);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		text[selectedItemIndex].setColor(sf::Color::Red);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::Blue);
	}

	//Loops back around
	else if (selectedItemIndex == maxDraw - 1)
	{
		text[selectedItemIndex].setColor(sf::Color::Black);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex = 0;
		text[selectedItemIndex].setColor(sf::Color::Red);
		hitboxes[selectedItemIndex].setFillColor(sf::Color::Blue);
	}
}

//Checks to see if the mouse is highlighting an option
void Menu::MouseCheck(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		checkMouse[i] = false;

		//Checks if the mouse is within the hitbox of selected item, if it is then select it
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

//Used to see what menu option is picked, case statement for each option
void Menu::MenuControls(sf::RenderWindow& window)
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		//
		//Checks if we are in the main menu
		//
		if (menuCheck == 0)
		{
			switch (event.type)
			{
				//If Keyboard/controller was press...
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				// Moves up on the menu with keyboard/controller
				case sf::Keyboard::Up:
					MoveUp();
					break;
				// Moves down on the menu with keyboard/controller
				case sf::Keyboard::Down:
					MoveDown();
					break;

				//If enter is press...
				case sf::Keyboard::Enter:

					//Checks what item is highlighted then executes it
					switch (GetPressedItem())
					{
					case 0:
						if (GetPressedItem() == 0)
							std::cout << "New game button has been pressed" << std::endl;
						break;
					case 1:
						if (GetPressedItem() == 1)
							std::cout << "Load game button has been pressed" << std::endl;
						break;
					case 2:
						if (GetPressedItem() == 2)
						{
							std::cout << "Option button has been pressed" << std::endl;
							menuCheck = 1;
							MenuTransition(storeWidth, storeHeight);
						}
						break;
					case 3:
						if (selectedItemIndex == 3)
							window.close();
						break;
					}

					break;
				}

				//If left mouse button is pressed...
			case sf::Event::MouseButtonPressed:

				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:

					//Checks to see if the mouse is hovering over that option, if it is then execute it
					switch (GetPressedItem())
					{
					case 0:
						if (checkMouse[0])
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
						if (checkMouse[3])
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

		//
		//Checks if we are in the options
		//
		else if (menuCheck == 1)
		{
			switch (event.type)
			{
				//Keyboard/Controller Checks
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
						if (GetPressedItem())
							std::cout << "Sound button has been pressed" << std::endl;
							std::cout << "Sound button has been pressed" << std::endl;
						break;
					case 1:
						if (GetPressedItem())
							std::cout << "Screen button has been pressed" << std::endl;
						break;
					case 2:
						if (GetPressedItem())
							std::cout << "Controls button has been pressed" << std::endl;
						break;
					case 3:
						if (GetPressedItem())
						{
							menuCheck = 0;
							MenuTransition(storeWidth, storeHeight);
						}
						break;
					}

					break;
				}
				//Mouse Checks
			case sf::Event::MouseButtonPressed:

				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:

					switch (GetPressedItem())
					{
					case 0:
						if (checkMouse[0])
							std::cout << "Sound button has been pressed" << std::endl;
						break;
					case 1:
						if (checkMouse[1])
							std::cout << "Screen button has been pressed" << std::endl;
						break;
					case 2:
						if (checkMouse[2])
							std::cout << "Controls button has been pressed" << std::endl;
						break;
					case 3:
						if (checkMouse[3])
						{
							menuCheck = 0;
							MenuTransition(storeWidth, storeHeight);
						}
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
}

//Used to Transition from different Menus using menuCheck
void Menu::MenuTransition(float width, float height)
{
	//Main Menu
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

		maxDraw = 4;
	}

	//Options Menu
	else if (menuCheck == 1)
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

		maxDraw = 4;
	}

	selectedItemIndex = 0;
}