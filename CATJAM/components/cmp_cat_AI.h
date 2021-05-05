#pragma once

#include "cmp_physics.h"
#include <chrono>
#include <ctime>

class CatAI : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  sf::Vector2f _wanderLocation;
  bool _grounded;
  float _groundspeed;

  bool _tPause = false;
  bool _startTime = false;
  bool _locationChosen = false;

  std::chrono::time_point<std::chrono::system_clock> _start;

  

  bool isGrounded() const;

public:
  void update(double dt) override;

  explicit CatAI(Entity* p, const sf::Vector2f& size);

  void PickWanderLocation();

  CatAI() = delete;
};
