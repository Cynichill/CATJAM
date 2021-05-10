#pragma once

#include "cmp_physics.h"
#include <chrono>

class Enemy : public PhysicsComponent {
protected:
	b2Vec2 _size;
	sf::Vector2f _maxVelocity;
	float _airspeed;
	bool deleteMe = false;

public:
	void update(double dt) override;

	explicit Enemy(Entity* p, const sf::Vector2f& size, sf::Vector2f maxVelocity);

	bool GetDelete();

	void SetDelete(bool del);

	Enemy() = delete;
};