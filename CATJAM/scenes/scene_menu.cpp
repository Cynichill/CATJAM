#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../components/cmp_physics.h"
//#include "Mouse.h"

using namespace std;
using namespace sf;

shared_ptr<TextComponent> t[4];
shared_ptr<ShapeComponent> boxes[4];


void MenuScene::Load() {
  cout << "Menu Load \n";
  {
	  //Box 1
	  shared_ptr<Entity> box = makeEntity();
	  boxes[0] = box->addComponent<ShapeComponent>();
	  box->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 0.9)); //Sets position of the hitboxes based on resolution

	  //Box 2
	  shared_ptr<Entity> box2 = makeEntity();
	  boxes[1] = box2->addComponent<ShapeComponent>();
	  box2->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 1.9)); //Sets position of the hitboxes based on resolution

	  //Box 3
	  shared_ptr<Entity> box3 = makeEntity();
	  boxes[2] = box3->addComponent<ShapeComponent>();
	  box3->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 2.9)); //Sets position of the hitboxes based on resolution

	  //Box 4
	  shared_ptr<Entity> box4 = makeEntity();
	  boxes[3] = box4->addComponent<ShapeComponent>();
	  box4->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 3.9)); //Sets position of the hitboxes based on resolution


	  for (int i = 0; i < 4; i++)
	  {
		  boxes[i]->setShape<sf::RectangleShape>(sf::Vector2f(250, 100));
		  boxes[i]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
	  }

	  //Text
	  string text[4];
	  text[0] = "New Game";
	  text[1] = "Load Game";
	  text[2] = "Options";
	  text[3] = "Quit Game";

	  shared_ptr<Entity> txt = makeEntity();

	  for (int i = 0; i < 4; ++i) {

		  t[i] = txt->addComponent<TextComponent>(
			  text[i]);
		  //Sets the text + hitboxes for the main menu
		  //We have an array of text and hitboxes which contain each item in the menu

		  t[i]->getText().setColor(sf::Color::Black); //Sets colour of text
		  t[i]->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * (1 + i))); //Sets position of the text based on resolution
	  }

	  selected = 0;
	  changeMenu = 0;
	  keyPressed = false;
	  maxDraw = 4;
  }
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

  MenuControls();
  MouseCheck();

  Scene::Update(dt);
}

void MenuScene::MouseCheck()
{
	for (int i = 0; i < 4; i++)
	{
		if (boxes[i]->getShape().getGlobalBounds().contains(Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()))))
		{
			selected = i;
		}
	}

	Highlight();
}

void MenuScene::Highlight()
{
	for (int i = 0; i < 4; i++)
	{
		if (selected == i)
		{
			boxes[i]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
			t[i]->getText().setColor(sf::Color::Red); //Sets colour of text
		}
		else
		{
			boxes[i]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
			t[i]->getText().setColor(sf::Color::Black); //Sets colour of text
		}
	}
}

void MenuScene::MenuControls()
{
	if (!keyPressed && sf::Keyboard::isKeyPressed(Keyboard::Up))
	{
		keyPressed = true;
		MoveUp();
	}

	if (!keyPressed && sf::Keyboard::isKeyPressed(Keyboard::Down))
	{
		keyPressed = true;
		MoveDown();
	}

	if (!keyPressed && sf::Keyboard::isKeyPressed(Keyboard::Enter))
	{
		keyPressed = true;
		MenuSelect();
	}

	for (int i = 0; i < 4; i++)
	{
		if (!keyPressed && sf::Mouse::isButtonPressed(Mouse::Left) && boxes[i]->getShape().getGlobalBounds().contains(Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()))))
		{
			keyPressed = true;
			MenuSelect();
		}
	}

	if (!sf::Keyboard::isKeyPressed(Keyboard::Down) && !sf::Keyboard::isKeyPressed(Keyboard::Up) && !sf::Keyboard::isKeyPressed(Keyboard::Enter) && !sf::Mouse::isButtonPressed(Mouse::Left))
	{
		keyPressed = false;
	}
}

void MenuScene::MoveUp()
{
	if (selected == 0)
	{
		selected = maxDraw - 1;
	}
	else
	{
		selected -= 1;
	}
}

void MenuScene::MoveDown()
{
	if (selected == maxDraw - 1)
	{
		selected = 0;
	}
	else
	{
		selected++;
	}
}

void MenuScene::MenuSelect()
{
	if (changeMenu == 0)
	{
		switch (selected)
		{
		case 0:
			Engine::ChangeScene(&level1);
			break;
		case 1:
			std::cout << "Load game button has been pressed" << std::endl;
			break;
		case 2:
			changeMenu = 1;
			MenuChange();
			break;
		case 3:
			Engine::GetWindow().close();
		}
	}

	if (changeMenu == 1)
	{
		switch (selected)
		{
		case 0:
			std::cout << "Sound button has been pressed" << std::endl;
			break;
		case 1:
			std::cout << "Screen button has been pressed" << std::endl;
			changeMenu = 2;
			MenuChange();
			break;
		case 2:
			std::cout << "Controls button has been pressed" << std::endl;
			break;
		case 3:
			changeMenu = 0;
			MenuChange();
		}
	}

	if (changeMenu == 2)
	{
		switch (selected)
		{
		case 0:
			std::cout << "Test" << std::endl;
			break;
		case 1:
			std::cout << "Test" << std::endl;
			break;
		case 2:
			changeMenu = 1;
			MenuChange();
			break;
		case 3:
			break;
		}
	}
}

void MenuScene::MenuChange()
{
	for (int i = 0; i < maxDraw; i++)
	{
		boxes[i]->getShape().setScale(sf::Vector2f(0, 0));
	}

	if (changeMenu == 0)
	{
		maxDraw = 4;

		t[0]->getText().setString("New Game");
		t[1]->getText().setString("Load Game");
		t[2]->getText().setString("Options");
		t[3]->getText().setString("Exit");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}
	}

	if (changeMenu == 1)
	{
		maxDraw = 4;

		t[0]->getText().setString("Sound");
		t[1]->getText().setString("Screen");
		t[2]->getText().setString("Controls");
		t[3]->getText().setString("Back");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}
	}

	if (changeMenu == 2)
	{
		maxDraw = 3;

		t[0]->getText().setString("Test");
		t[1]->getText().setString("Test");
		t[2]->getText().setString("Exit");

		t[3]->getText().setString("");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}

	}

	selected = 0;
}





