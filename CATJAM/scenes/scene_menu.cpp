#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <fstream>
#include "../components/cmp_physics.h"
#include "SFML/Audio.hpp"

using namespace std;
using namespace sf;

shared_ptr<TextComponent> t[4];
shared_ptr<TextComponent> title;
shared_ptr<ShapeComponent> boxes[4];
shared_ptr<Entity> box;
shared_ptr<Entity> box2;
shared_ptr<Entity> box3;
shared_ptr<Entity> box4;
sf::Music music;

void MenuScene::Load() {
  cout << "Menu Load \n";
  {
	  //Box 1
	  box = makeEntity();
	  boxes[0] = box->addComponent<ShapeComponent>();
	  box->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 0.9)); //Sets position of the hitboxes based on resolution

	  //Box 2
	  box2 = makeEntity();
	  boxes[1] = box2->addComponent<ShapeComponent>();
	  box2->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 1.9)); //Sets position of the hitboxes based on resolution

	  //Box 3
	  box3 = makeEntity();
	  boxes[2] = box3->addComponent<ShapeComponent>();
	  box3->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 2.9)); //Sets position of the hitboxes based on resolution

	  //Box 4
	  box4 = makeEntity();
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

	  title = txt->addComponent<TextComponent>();
	  title->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 - 100, 50));
	  title->SetText("Cat-atonic");
	  title->getText().setScale(2,2);

	  ifstream file("resolutionFile.txt");
	  if (!file)
	  {
		  cout << "Failed to find file" << endl;
	  }
	  else
	  {
		  //CHECK FILE NOT EMPTY
		  file.seekg(0, ios::end);
		  size_t size = file.tellg();
		  if (size == 0)
		  {
			  cout << "File is empty\n";
		  }
		  else
		  {
			  file.seekg(0, ios::beg);
			  float w = 0, h = 0;
			  bool s, v;
			  if (file >> w >> h)
			  {
				  widthStore = w;
				  heightStore = h;

			  }
			  else
			  {
				  cout << "Failed to load file" << endl;
			  }
		  }
	  }

	  if (widthStore == 800 && heightStore == 600)
	  {
		  //Changes the position of boxes with relation to resolution
		  box->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 0.9));
		  box2->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 1.9));
		  box3->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 2.9));
		  box4->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 3.9));

		  title->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 - 100, 25));

		  for (int i = 0; i < 4; ++i) {

			  t[i]->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.5, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * (1 + i))); //Sets position of the text based on resolution
		  }
	  }

	  selected = 0;
	  changeMenu = 0;
	  keyPressed = true;
	  maxDraw = 4;
	  controller = false;

	  //Music
	  if (!music.openFromFile("res/music/menu.wav"))
	  {
		  cout << "Failed to load music" << endl;
		  //error
	  }
	  music.setLoop(true);
	  music.play();
	  music.setVolume(25);
  }
  setLoaded(true);
}

//Unloads the scene
void MenuScene::UnLoad()
{
	//Resets all the pointers
	for (int i = 0; i < 4; ++i) 
	{
		t[i].reset();
		boxes[i].reset();
	}
	box.reset();
	box2.reset();
	box3.reset();
	box4.reset();
	title.reset();
	music.stop();
	Scene::UnLoad();
}

//Updates each frame
void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

	MouseCheck();
	MenuControls();

  Scene::Update(dt);
}

//Checks to see if the mouse is over an option
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

//Highlights the option selected
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

//Controls the menu with Keyboards and mouse clicks
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

//Moves up in the menu
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

//Moves down in the menu
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

//Checks where we are in the Menu
void MenuScene::MenuSelect()
{
	/*
	New Game
	Load Game
	Options
	Exit
	*/
	if (changeMenu == 0)
	{
		switch (selected)
		{
		case 0:
			DeleteSaveData();
			UnLoad();
			Engine::ChangeScene(&level1);
			break;
		case 1:
			UnLoad();
			Engine::ChangeScene(&level1);
			break;
		case 2:
			changeMenu = 1;
			MenuChange();
			break;
		case 3:
			Engine::GetWindow().close();
		}
	}

	/*
	Sound
	Screen
	Controls
	Back
	*/
	else if (changeMenu == 1)
	{
		switch (selected)
		{
		case 0:
			std::cout << "Sound button has been pressed" << std::endl;
			break;
		case 1:
			changeMenu = 2;
			MenuChange();
			break;
		case 2:
			changeMenu = 6;
			MenuChange();
			break;
		case 3:
			changeMenu = 0;
			MenuChange();
		}
	}

	/*
	Window
	Resolution
	Vsync
	Back
	*/
	else if (changeMenu == 2)
	{
		switch (selected)
		{
		case 0:
			changeMenu = 4;
			MenuChange();
			break;
		case 1:
			changeMenu = 3;
			MenuChange();
			break;
		case 2:
			changeMenu = 5;
			MenuChange();
			break;
		case 3:
			changeMenu = 1;
			MenuChange();
			break;
		}
	}

	/*
	1920x1080
	1600x900
	800x600
	Back
	*/
	else if (changeMenu == 3)
	{
		switch (selected)
		{
		case 0:
			widthStore = 1920;
			heightStore = 1080;
			setSize();
			break;
		case 1:
			widthStore = 1600;
			heightStore = 900;
			setSize();
			break;
		case 2:
			widthStore = 800;
			heightStore = 600;
			setSize();
			break;
		case 3:
			changeMenu = 2;
			MenuChange();
			break;
		}
	}

	/*
	Fullscreen
	Windowed
	Back
	*/
	else if (changeMenu == 4)
	{
		switch (selected)
		{
		case 0:
			if (fullscreen)
			{
				break;
			}
			else
			fullscreen = true;
			setSize();
			break;
		case 1:
			if (fullscreen)
			{
				fullscreen = false;
				setSize();
				break;
			}
			else
			break;
		case 2:
			changeMenu = 2;
			MenuChange();
			break;
		case 3:
			break;
		}
	}

	/*
	On
	Off
	Back
	*/
	else if (changeMenu == 5)
	{
		switch (selected)
		{
		case 0:
			vsync = true;
			setSize();
			break;
		case 1:
			vsync = false;
			setSize();
			break;
		case 2:
			changeMenu = 2;
			MenuChange();
			break;
		case 3:
			break;
		}
	}

	/*
	Mouse
	Controller
	Back
	*/
	else if (changeMenu == 6)
	{
		switch (selected)
		{
		case 0:
		
			break;
		case 1:
		
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

//Changes the menu to the right one
void MenuScene::MenuChange()
{
	for (int i = 0; i < maxDraw; i++)
	{
		boxes[i]->getShape().setScale(sf::Vector2f(0, 0));
	}

	//Main Menu
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
		title->SetText("Cat-atonic");
	}

	//Options Menu
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
		title->SetText("");
	}

	//Screens Menu
	if (changeMenu == 2)
	{
		maxDraw = 4;

		t[0]->getText().setString("Window");
		t[1]->getText().setString("Resolution");
		t[2]->getText().setString("Vsync");
		t[3]->getText().setString("Back");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}
		title->SetText("");
	}

	//Resolutions Menu
	if (changeMenu == 3)
	{
		maxDraw = 4;

		t[0]->getText().setString("1920x1080");
		t[1]->getText().setString("1600x900");
		t[2]->getText().setString("800x600");
		t[3]->getText().setString("Back");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}
		title->SetText("");
	}

	//Windowed Menu
	if (changeMenu == 4)
	{
		maxDraw = 3;

		t[0]->getText().setString("Fullscreen");
		t[1]->getText().setString("Windowed");
		t[2]->getText().setString("Back");

		t[3]->getText().setString("");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}
		title->SetText("");
	}

	//Vsync Menu
	if (changeMenu == 5)
	{
		maxDraw = 3;

		t[0]->getText().setString("On");
		t[1]->getText().setString("Off");
		t[2]->getText().setString("Back");

		t[3]->getText().setString("");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}
		title->SetText("");
	}

	if (changeMenu == 6)
	{
		maxDraw = 3;

		t[0]->getText().setString("Mouse");
		t[1]->getText().setString("Keyboard");
		t[2]->getText().setString("Back");

		t[3]->getText().setString("");

		for (int i = 0; i < maxDraw; i++)
		{
			boxes[i]->getShape().setScale(sf::Vector2f(1, 1));
		}
		title->SetText("");
	}

	selected = 0;
	Highlight();
}

//Sets the correct size for the window
void MenuScene::setSize()
{
	//Checks if fullscreen is enabled
	if (fullscreen)
	{
		Engine::GetWindow().create(sf::VideoMode(widthStore, heightStore), "SFML Window", sf::Style::Fullscreen);
	}
	else
	{
		Engine::GetWindow().create(sf::VideoMode(widthStore, heightStore), "SFML Window");
	}

	//Changes the position of boxes with relation to resolution
	box->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 0.9));
	box2->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 1.9));
	box3->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 2.9));
	box4->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.1, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 3.9));
	title->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 - 100, 50));

	//Changes the position of texes with relation to resolution
	for (int i = 0; i < 4; i++)
	{
		t[i]->getText().setColor(sf::Color::Black); //Sets colour of text
		t[i]->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * (1 + i))); //Sets position of the text based on resolution
	}

	if (widthStore == 800 && heightStore == 600)
	{
		//Changes the position of boxes with relation to resolution
		box->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 0.9));
		box2->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 1.9));
		box3->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 2.9));
		box4->setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.6, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 3.9));
		title->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2 - 100, 25));
		for (int i = 0; i < 4; ++i) {
			t[i]->getText().setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.5, Engine::getWindowSize().y / (MAX_NUMBER_OF_ITEMS + 1) * (1 + i))); //Sets position of the text based on resolution
		}
	}

	Engine::setVsync(vsync);

	std::ofstream saveFile("resolutionFile.txt", std::ofstream::out);
	saveFile << widthStore << " ";
	saveFile << heightStore << " ";
	saveFile.close();

}

bool MenuScene::controllerType()
{
	return controller;
}

void MenuScene::DeleteSaveData()
{
	ofstream myfile("catFile.txt");
	ofstream myfile2("playerFile.txt");
	ofstream myfile3("clockFile.txt");
	ofstream myfile4("invFile.txt");

	if (!myfile)
	{
		cout << "Failed to find file" << endl;
	}
	{
		myfile.open("catFile.txt");
		remove("catFile.txt");
	}
	myfile.close();

	if (!myfile2)
	{
		cout << "Failed to find file" << endl;
	}
	else
	{
		myfile2.open("playerFile.txt");
		remove("playerFile.txt");
	}

	myfile2.close();

	if (!myfile3)
	{
		cout << "Failed to find file" << endl;
	}
	else
	{
		myfile3.open("clockFile.txt");
		remove("clockFile.txt");
	}

	myfile3.close();

	if (!myfile4)
	{
		cout << "Failed to find file" << endl;
	}
	else
	{
		myfile4.open("invFile.txt");
		remove("invFile.txt");
	}

	myfile4.close();
}