#include "scene_level2.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

shared_ptr<TextComponent> stillUiText[2];
shared_ptr<TextComponent> uiText[4];
shared_ptr<ShapeComponent> stillUi[2];
shared_ptr<ShapeComponent> ui[4];
shared_ptr<Entity> date;
shared_ptr<Entity> currency;
shared_ptr<Entity> options;
shared_ptr<Entity> tutorial;
shared_ptr<Entity> shop;
shared_ptr<Entity> inventory;
shared_ptr<SpriteComponent> cursorEntity;
std::shared_ptr<Entity> cursorSprite;

void Level2Scene::Load() 
{

	//Sets the shape for the shop box
	sf::ConvexShape shopShape;
	shopShape.setPointCount(5);
	shopShape.setPoint(0, sf::Vector2f(0, 0));
	shopShape.setPoint(1, sf::Vector2f(60, 0));
	shopShape.setPoint(2, sf::Vector2f(50, 30));
	shopShape.setPoint(3, sf::Vector2f(-50, 30));
	shopShape.setPoint(4, sf::Vector2f(-60, 0));

	//Sets the shape for the inventory box
	sf::ConvexShape inventoryShape;
	inventoryShape.setPointCount(5);
	inventoryShape.setPoint(0, sf::Vector2f(0, 0));
	inventoryShape.setPoint(1, sf::Vector2f(60, 0));
	inventoryShape.setPoint(2, sf::Vector2f(50, 30));
	inventoryShape.setPoint(3, sf::Vector2f(-50, 30));
	inventoryShape.setPoint(4, sf::Vector2f(-60, 0));


	//Date Box
	date = makeEntity();
	stillUi[0] = date->addComponent<ShapeComponent>();
	date->setPosition(sf::Vector2f(Engine::getWindowSize().x - 180, 2)); //Sets position of the hitboxes based on resolution
	stillUi[0]->setShape<sf::RectangleShape>(sf::Vector2f(175, 40));
	stillUi[0]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes


	//Currency Box
	currency = makeEntity();
	stillUi[1] = currency->addComponent<ShapeComponent>();
	currency->setPosition(sf::Vector2f(Engine::getWindowSize().x - 180, 44)); //Sets position of the hitboxes based on resolution
	stillUi[1]->setShape<sf::RectangleShape>(sf::Vector2f(175, 40));
	stillUi[1]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes


	//Options Box
	options = makeEntity();
	ui[0] = options->addComponent<ShapeComponent>();
	options->setPosition(sf::Vector2f(Engine::getWindowSize().x - 175, 88)); //Sets position of the hitboxes based on resolution
	ui[0]->setShape<sf::RectangleShape>(sf::Vector2f(50, 50));


	//Tutorial Box
	tutorial = makeEntity();
	ui[1] = tutorial->addComponent<ShapeComponent>();
	tutorial->setPosition(sf::Vector2f(Engine::getWindowSize().x - 60, 88)); //Sets position of the hitboxes based on resolution
	ui[1]->setShape<sf::RectangleShape>(sf::Vector2f(50, 50));


	//Shop
	shop = makeEntity();
	ui[2] = shop->addComponent<ShapeComponent>();
	shop->setPosition(sf::Vector2f(Engine::getWindowSize().x/2 - 100, 0)); //Sets position of the hitboxes based on resolution
	ui[2]->setShape<sf::ConvexShape>(shopShape);


	//Inventory
	inventory = makeEntity();
	ui[3] = inventory->addComponent<ShapeComponent>();
	inventory->setPosition(sf::Vector2f(Engine::getWindowSize().x/2 + 100, 0)); //Sets position of the hitboxes based on resolution
	ui[3]->setShape<sf::ConvexShape>(inventoryShape);


	
	for (int i = 0; i < 2; i++)
	{
		ui[i]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
	}


	//Sets the text for highlightable stuff
	string text[4];
	text[0] = "T";
	text[1] = "O";
	text[2] = "Shop";
	text[3] = "Inventory";

	//Sets the text for time and currancy
	string stillText[2];
	stillText[0] = "Test";
	stillText[1] = "Test";

	shared_ptr<Entity> txt = makeEntity();

	for (int i = 0; i < 4; ++i) {

		uiText[i] = txt->addComponent<TextComponent>(
			text[i]);
		//Sets the text
		//We have an array of text which contain each item in the menu

		uiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
	}

	for (int i = 0; i < 2; ++i) {

		stillUiText[i] = txt->addComponent<TextComponent>(stillText[i]);

		stillUiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
	}

	stillUiText[0]->getText().setPosition(date->getPosition()); //Sets position of Date text
	stillUiText[1]->getText().setPosition(currency->getPosition()); //Sets position Currency text

	uiText[0]->getText().setPosition(options->getPosition()); //Sets position of Options text
	uiText[1]->getText().setPosition(tutorial->getPosition()); //Sets position of Tutorial text

	uiText[2]->getText().setPosition(sf::Vector2f(shop->getPosition().x-20, shop->getPosition().y)); //Sets position of Shop text
	uiText[3]->getText().setPosition(sf::Vector2f(inventory->getPosition().x-40, inventory->getPosition().y)); //Sets position of Inventory Text

	uiText[2]->getText().setScale(0.5, 0.5); //Scales the text so it fits in box
	uiText[3]->getText().setScale(0.5, 0.5); //Scales the text so it fits in box

	std::shared_ptr<Texture> cursor = std::make_shared<sf::Texture>();;
	cursorSprite = makeEntity();

	if (!cursor->loadFromFile("res/sprites/mouse.png"))
	{
		//Error
	}
	cursorEntity = cursorSprite->addComponent<SpriteComponent>();
	cursorEntity->setTexture(cursor);
	cursorEntity->getSprite().setPosition(0,0);
	cursorSpeed = 0.15f;


	//Setting Variables
	keyPressed = false;
	menuDropped = false;
	shopOpen = false;
	selected = false;
	hideMouse = false;
	controller = false;
	
	cout << " Scene 2 Load Done" << endl;
	setLoaded(true);
}

void Level2Scene::UnLoad() 
{
	//Resets all the pointers
	for (int i = 0; i < 4; ++i)
	{
		ui[i].reset();
		uiText[i].reset();
	}
	stillUi[0].reset();
	stillUi[1].reset();
	stillUiText[0].reset();
	stillUiText[1].reset();
	date.reset();
	currency.reset();
	options.reset();
	tutorial.reset(); 
	shop.reset();
	inventory.reset();
	cursorEntity.reset();
	cursorSprite.reset();

  cout << "Scene 2 UnLoad" << endl;
  Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) 
{
	Engine::GetWindow().setMouseCursorVisible(hideMouse);
	if (!controller)
	{
		MouseUpdate();
	}
	else if (controller)
	{
		KeyboardUpdate();
	}
	Controls();
	Highlight();
	Scene::Update(dt);
}

void Level2Scene::Render() 
{
	Scene::Render();
}

void Level2Scene::Highlight()
{
	for (int i = 0; i < 4; i++)
	{
		if (ui[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
		{
			ui[i]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
			uiText[i]->getText().setColor(sf::Color::Red); //Sets colour of text
		}
		else
		{
			ui[i]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
			uiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
		}
	}

	if (shopOpen && menuDropped)
	{
		ui[2]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
		uiText[2]->getText().setColor(sf::Color::Red); //Sets colour of text
	}

	else if (!shopOpen && menuDropped)
	{
		ui[3]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
		uiText[3]->getText().setColor(sf::Color::Red); //Sets colour of text
	}
}

void Level2Scene::Controls()
{
	if (!controller)
	{
		for (int i = 2; i < 4; i++)
		{
			if (!keyPressed && sf::Mouse::isButtonPressed(Mouse::Left) && ui[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
			{
				keyPressed = true;

				if (i == 2)
				{
					shopOpen = true;
				}
				if (i == 3)
				{
					shopOpen = false;
				}

				MenuDrop();
			}
		}
	}

	if (controller)
	{
		for (int i = 2; i < 4; i++)
		{
			if (!keyPressed && sf::Keyboard::isKeyPressed(Keyboard::Enter) && ui[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
			{
				keyPressed = true;

				if (i == 2)
				{
					shopOpen = true;
				}
				if (i == 3)
				{
					shopOpen = false;
				}

				MenuDrop();
			}
		}
	}

	if (!keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
	{
		keyPressed = true;
		hideMouse = !hideMouse;
	}

	if (!sf::Mouse::isButtonPressed(Mouse::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::F3) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		keyPressed = false;
	}
}

void Level2Scene::MenuDrop()
{
	if (menuDropped)
	{
		if (selected == shopOpen)
		{
			menuDropped = false;
			shop->setPosition(sf::Vector2f(ui[2]->getShape().getPosition().x, ui[2]->getShape().getPosition().y - 500));
			inventory->setPosition(sf::Vector2f(ui[3]->getShape().getPosition().x, ui[3]->getShape().getPosition().y - 500));
			uiText[2]->getText().setPosition(sf::Vector2f(shop->getPosition().x - 20, shop->getPosition().y)); //Sets position of the text based on resolution
			uiText[3]->getText().setPosition(sf::Vector2f(inventory->getPosition().x - 40, inventory->getPosition().y)); //Sets position of the text based on resolution
		}
		else
		{
			selected = shopOpen;
		}

	}
	else if (!menuDropped)
	{
		menuDropped = true;
		shop->setPosition(sf::Vector2f(ui[2]->getShape().getPosition().x, ui[2]->getShape().getPosition().y + 500));
		inventory->setPosition(sf::Vector2f(ui[3]->getShape().getPosition().x, ui[3]->getShape().getPosition().y + 500));
		uiText[2]->getText().setPosition(sf::Vector2f(shop->getPosition().x - 20, shop->getPosition().y)); //Sets position of the text based on resolution
		uiText[3]->getText().setPosition(sf::Vector2f(inventory->getPosition().x - 40, inventory->getPosition().y)); //Sets position of the text based on resolution
		selected = shopOpen;
	}
}

void Level2Scene::MouseUpdate()
{
	cursorSprite->setPosition(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow()).x - 7.0f, sf::Mouse::getPosition(Engine::GetWindow()).y));
}

void Level2Scene::KeyboardUpdate()
{
	//Move cursor Down
	if (cursorSprite->getPosition().y <= Engine::getWindowSize().y - 16.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x, cursorSprite->getPosition().y + cursorSpeed));
	}

	//Move cursor Up
	if (cursorSprite->getPosition().y >= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x, cursorSprite->getPosition().y - cursorSpeed));
	}

	//Move cursor Right
	if (cursorSprite->getPosition().x <= Engine::getWindowSize().x - 16.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x + cursorSpeed, cursorSprite->getPosition().y));
	}

	//Move cursor Left
	if (cursorSprite->getPosition().x >= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		cursorSprite->setPosition(sf::Vector2f(cursorSprite->getPosition().x - cursorSpeed, cursorSprite->getPosition().y));
	}
}