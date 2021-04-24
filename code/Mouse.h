#pragma once

class Mouse
{
public:
	Mouse(float width, float height);
	~Mouse();
	void draw(sf::RenderWindow& window);
	void MoveMouse(sf::RenderWindow& window);
	void MoveKey();
	sf::Vector2f getPosition();
private:
	sf::Texture cursor;
	sf::Sprite cursorSprite;
	float widthStore;
	float heightStore;
	float cursorSpeed;
};