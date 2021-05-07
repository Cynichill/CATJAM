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

shared_ptr<TextComponent> stillUiText[4];
shared_ptr<TextComponent> uiText[4];
shared_ptr<ShapeComponent> stillUi[4];
shared_ptr<ShapeComponent> ui[4];
shared_ptr<ShapeComponent> slots[6];
shared_ptr<ShapeComponent> box;
shared_ptr<Entity> toys;
shared_ptr<Entity> food;
shared_ptr<Entity> boxEntity;
shared_ptr<Entity> date;
shared_ptr<Entity> currency;
shared_ptr<Entity> options;
shared_ptr<Entity> tutorial;
shared_ptr<Entity> shop;
shared_ptr<Entity> inventory;
shared_ptr<Entity> slot1;
shared_ptr<Entity> slot2;
shared_ptr<Entity> slot3;
shared_ptr<Entity> slot4;
shared_ptr<Entity> slot5;
shared_ptr<Entity> slot6;
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

	//Background for inventory & shop
	boxEntity = makeEntity();
	box = boxEntity->addComponent<ShapeComponent>();
	boxEntity->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 * 0.5, 0)); //Sets position of the hitboxes based on resolution
	box->setShape<sf::RectangleShape>(sf::Vector2f(400, 499));
	box->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
	box->getShape().setOutlineColor(sf::Color::Black);

	boxEntity->setVisible(false);



	//Date Box
	date = makeEntity();
	stillUi[0] = date->addComponent<ShapeComponent>();
	date->setPosition(sf::Vector2f(Engine::getWindowSize().x - 180, 2)); //Sets position of the hitboxes based on resolution
	stillUi[0]->setShape<sf::RectangleShape>(sf::Vector2f(175, 40));
	stillUi[0]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
	stillUi[0]->getShape().setOutlineColor(sf::Color::Black);


	//Currency Box
	currency = makeEntity();
	stillUi[1] = currency->addComponent<ShapeComponent>();
	currency->setPosition(sf::Vector2f(Engine::getWindowSize().x - 180, 44)); //Sets position of the hitboxes based on resolution
	stillUi[1]->setShape<sf::RectangleShape>(sf::Vector2f(175, 40));
	stillUi[1]->getShape().setFillColor(sf::Color::White); //Sets Colour of the hitboxes
	stillUi[1]->getShape().setOutlineColor(sf::Color::Black);


	//Food Box
	food = makeEntity();
	stillUi[2] = food->addComponent<ShapeComponent>();
	food->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, 5)); //Sets position of the hitboxes based on resolution
	stillUi[2]->setShape<sf::RectangleShape>(sf::Vector2f(150, 50));
	stillUi[2]->getShape().setFillColor(sf::Color::Black); //Sets Colour of the hitboxes
	stillUi[2]->getShape().setOutlineColor(sf::Color::Black);
	stillUi[2]->getShape().setScale(0, 0);

	//Toys Box
	toys = makeEntity();
	stillUi[3] = toys->addComponent<ShapeComponent>();
	toys->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, 5)); //Sets position of the hitboxes based on resolution
	stillUi[3]->setShape<sf::RectangleShape>(sf::Vector2f(150, 50));
	stillUi[3]->getShape().setFillColor(sf::Color::Black); //Sets Colour of the hitboxes
	stillUi[3]->getShape().setOutlineColor(sf::Color::Black);
	stillUi[3]->getShape().setScale(0, 0);

	//Options Box
	options = makeEntity();
	ui[0] = options->addComponent<ShapeComponent>();
	options->setPosition(sf::Vector2f(Engine::getWindowSize().x - 175, 88)); //Sets position of the hitboxes based on resolution
	ui[0]->setShape<sf::RectangleShape>(sf::Vector2f(50, 50));
	ui[0]->getShape().setOutlineColor(sf::Color::Black);


	//Tutorial Box
	tutorial = makeEntity();
	ui[1] = tutorial->addComponent<ShapeComponent>();
	tutorial->setPosition(sf::Vector2f(Engine::getWindowSize().x - 60, 88)); //Sets position of the hitboxes based on resolution
	ui[1]->setShape<sf::RectangleShape>(sf::Vector2f(50, 50));
	ui[1]->getShape().setOutlineColor(sf::Color::Black);


	//Shop
	shop = makeEntity();
	ui[2] = shop->addComponent<ShapeComponent>();
	shop->setPosition(sf::Vector2f(Engine::getWindowSize().x/2 - 100, 0)); //Sets position of the hitboxes based on resolution
	ui[2]->setShape<sf::ConvexShape>(shopShape);
	ui[2]->getShape().setOutlineColor(sf::Color::Black);


	//Inventory
	inventory = makeEntity();
	ui[3] = inventory->addComponent<ShapeComponent>();
	inventory->setPosition(sf::Vector2f(Engine::getWindowSize().x/2 + 100, 0)); //Sets position of the hitboxes based on resolution
	ui[3]->setShape<sf::ConvexShape>(inventoryShape);
	ui[3]->getShape().setOutlineColor(sf::Color::Black);


	//Slots for Inventory + Shop
	slot1 = makeEntity();
	slot2 = makeEntity();
	slot3 = makeEntity();
	slot4 = makeEntity();
	slot5 = makeEntity();
	slot6 = makeEntity();

	slot1->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, boxEntity->getPosition().y + 175)); //Sets position of the hitboxes based on resolution
	slot2->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, boxEntity->getPosition().y + 275)); //Sets position of the hitboxes based on resolution
	slot3->setPosition(sf::Vector2f(boxEntity->getPosition().x + 25, boxEntity->getPosition().y + 375)); //Sets position of the hitboxes based on resolution
	slot4->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, boxEntity->getPosition().y + 175)); //Sets position of the hitboxes based on resolution
	slot5->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, boxEntity->getPosition().y + 275)); //Sets position of the hitboxes based on resolution
	slot6->setPosition(sf::Vector2f(boxEntity->getPosition().x + 225, boxEntity->getPosition().y + 375)); //Sets position of the hitboxes based on resolution

	slots[0] = slot1->addComponent<ShapeComponent>();
	slots[1] = slot2->addComponent<ShapeComponent>();
	slots[2] = slot3->addComponent<ShapeComponent>();
	slots[3] = slot4->addComponent<ShapeComponent>();
	slots[4] = slot5->addComponent<ShapeComponent>();
	slots[5] = slot6->addComponent<ShapeComponent>();

	for (int i = 0; i < 6; i++)
	{
		slots[i]->setShape<sf::RectangleShape>(sf::Vector2f(150, 50));
		slots[i]->getShape().setFillColor(sf::Color::Black);
		slots[i]->getShape().setOutlineColor(sf::Color::Black);
		slots[i]->getShape().setScale(0, 0);
	}

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
	string stillText[4];
	stillText[0] = "Test";
	stillText[1] = "Test";
	stillText[2] = "";
	stillText[3] = "";

	shared_ptr<Entity> txt = makeEntity();

	for (int i = 0; i < 4; ++i) {

		uiText[i] = txt->addComponent<TextComponent>(
			text[i]);
		//Sets the text
		//We have an array of text which contain each item in the menu

		uiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
	}

	for (int i = 0; i < 4; ++i) {

		stillUiText[i] = txt->addComponent<TextComponent>(stillText[i]);

		stillUiText[i]->getText().setColor(sf::Color::Black); //Sets colour of text
	}

	stillUiText[0]->getText().setPosition(date->getPosition()); //Sets position of Date text
	stillUiText[1]->getText().setPosition(currency->getPosition()); //Sets position Currency text

	stillUiText[2]->getText().setPosition(food->getPosition()); //Sets position of Date text
	stillUiText[3]->getText().setPosition(toys->getPosition()); //Sets position Currency text
	stillUiText[2]->getText().setColor(sf::Color::White); //Sets colour of text
	stillUiText[3]->getText().setColor(sf::Color::White); //Sets colour of text

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
		stillUi[i].reset();
		stillUiText[i].reset();
	}
	date.reset();
	currency.reset();
	options.reset();
	tutorial.reset(); 
	shop.reset();
	inventory.reset();
	cursorEntity.reset();
	cursorSprite.reset();
	box.reset();
	boxEntity.reset();
	food.reset();
	toys.reset();

	for (int i = 0; i < 6; i++)
	{
		slots[i].reset();
	}

	slot1.reset();
	slot2.reset();
	slot3.reset();
	slot4.reset();
	slot5.reset();
	slot6.reset();

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

	for (int i = 0; i < 6; i++)
	{
		if (slots[i]->getShape().getGlobalBounds().contains(cursorSprite->getPosition()))
		{
			slots[i]->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes
		}
		else
		{
			slots[i]->getShape().setFillColor(sf::Color::Black); //Sets Colour of the hitboxes
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

			UnLoadMenu();

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

		LoadMenu();
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

void Level2Scene::LoadMenu()
{
	boxEntity->setVisible(true);

	for (int i = 0; i < 6; i++)
	{
		slots[i]->getShape().setScale(1, 1);
	}
	stillUi[2]->getShape().setScale(1, 1);
	stillUi[3]->getShape().setScale(1, 1);
	stillUiText[2]->SetText("Food");
	stillUiText[3]->SetText("Toys");
}

void Level2Scene::UnLoadMenu()
{

	boxEntity->setVisible(false);

	for (int i = 0; i < 6; i++)
	{
		slots[i]->getShape().setScale(0, 0);
	}
	stillUi[2]->getShape().setScale(0, 0);
	stillUi[3]->getShape().setScale(0, 0);
	stillUiText[2]->SetText("");
	stillUiText[3]->SetText("");
}