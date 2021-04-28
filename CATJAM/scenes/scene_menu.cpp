#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../components/cmp_physics.h"
//#include "Mouse.h"

using namespace std;
using namespace sf;

void MenuScene::Load() {
  cout << "Menu Load \n";
  {

	  shared_ptr<Entity> box = makeEntity();

	  auto h = box->addComponent<ShapeComponent>();
	  h->setShape<sf::RectangleShape>(sf::Vector2f(250, 100));

	  box->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS) * 0.9)); //Sets position of the hitboxes based on resolution
	  h->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes

	  shared_ptr<Entity> box2 = makeEntity();

	  h = box2->addComponent<ShapeComponent>();
	  h->setShape<sf::RectangleShape>(sf::Vector2f(250, 100));

	  box2->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS) * 1.9)); //Sets position of the hitboxes based on resolution
	  h->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes

	  shared_ptr<Entity> box3 = makeEntity();

	  h = box3->addComponent<ShapeComponent>();
	  h->setShape<sf::RectangleShape>(sf::Vector2f(250, 100));

	  box3->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS) * 2.9)); //Sets position of the hitboxes based on resolution
	  h->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes

	  shared_ptr<Entity> box4 = makeEntity();

	  h = box4->addComponent<ShapeComponent>();
	  h->setShape<sf::RectangleShape>(sf::Vector2f(250, 100));

	  box4->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS) * 3.9)); //Sets position of the hitboxes based on resolution
	  h->getShape().setFillColor(sf::Color::Blue); //Sets Colour of the hitboxes


	  string text[4];
	  text[0] = "New Game";
	  text[1] = "Load Game";
	  text[2] = "Options";
	  text[3] = "Quit Game";

	for (int i = 0; i < 4; ++i) {
		shared_ptr<Entity> txt = makeEntity();

		auto t = txt->addComponent<TextComponent>(
			text[i]);
		//Sets the text + hitboxes for the main menu
		//We have an array of text and hitboxes which contain each item in the menu

		t->getText().setColor(sf::Color::Red); //Sets colour of text
		t->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * (1 + i))); //Sets position of the text based on resolution
	}

  }
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

  if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
    Engine::ChangeScene(&level1);
  }

  Scene::Update(dt);
}
