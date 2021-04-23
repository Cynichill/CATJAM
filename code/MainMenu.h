#pragma once
#include <SFML/Graphics.hpp>

#define MAX_NUMBER_OF_ITEMS 4

class Menu
{
public:
	Menu(float width, float height);
	~Menu();

	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	void MouseCheck(sf::RenderWindow& window);
	int GetPressedItem() { return selectedItemIndex; }
	void MenuControls(sf::RenderWindow& window);
	void MenuTransition(float width, float height);

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text text[MAX_NUMBER_OF_ITEMS];
	sf::RectangleShape hitboxes[MAX_NUMBER_OF_ITEMS];
	bool checkMouse[MAX_NUMBER_OF_ITEMS];
	int menuCheck;
	float storeWidth;
	float storeHeight;
};