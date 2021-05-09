#pragma once

#include "cmp_physics.h"
#include <chrono>

class Enemy : public PhysicsComponent {
protected:
	b2Vec2 _size;
	sf::Vector2f _maxVelocity;
	sf::Vector2f _wanderLocation;
	sf::Vector2f _target;
	bool _grounded;
	float _groundspeed;
	bool deleteMe = false;

	bool isGrounded() const;

public:
	void update(double dt) override;

	explicit Enemy(Entity* p, const sf::Vector2f& size, sf::Vector2f maxVelocity);

	bool GetDelete();

	Enemy() = delete;
};