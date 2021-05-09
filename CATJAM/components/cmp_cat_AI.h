#pragma once

#include "cmp_physics.h"
#include <chrono>

class CatAI : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  sf::Vector2f _wanderLocation;
  sf::Vector2f _target;
  float _groundspeed;
  std::string state = "WANDER";

  bool _tPause = false;
  bool _startTime = false;
  bool _locationChosen = false;
  bool _minigameVer = false;

  std::chrono::time_point<std::chrono::system_clock> _start;

public:
  void update(double dt) override;

  explicit CatAI(Entity* p, const sf::Vector2f& size, bool minigameVer, sf::Vector2f maxVelocity);

  void PickTarget(std::string targ, sf::Vector2f& targLocation = sf::Vector2f(0.0f, 0.0f));

  sf::Vector2f GetTarget();
  bool GetPause();

  void SetChosen(bool chos);

  bool GetChosen();


  CatAI() = delete;
};
