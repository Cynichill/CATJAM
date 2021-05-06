#include "cmp_cat_AI.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

bool CatAI::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void CatAI::update(double dt) {

  const auto pos = _parent->getPosition();

  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }
  
  //If not close to target location, move towards it on the correct axis
  if ((pos.x < _wanderLocation.x - 5 && pos.x < _wanderLocation.x + 5) || (pos.x > _wanderLocation.x - 5 && pos.x > _wanderLocation.x + 5)) {

    // Moving Either Left or Right
    if (pos.x < _wanderLocation.x) {
      if (getVelocity().x < _maxVelocity.x)
        impulse({(float)(dt * _groundspeed), 0});
    } else if (pos.x > _wanderLocation.x) {
      if (getVelocity().x > -_maxVelocity.x)
        impulse({-(float)(dt * _groundspeed), 0});
    }
  } else {
    // Dampen X axis movement
    dampen({0.9f, 1.0f});
  }

  if ((pos.y < _wanderLocation.y - 5 && pos.y < _wanderLocation.y + 5) || (pos.y > _wanderLocation.y - 5 && pos.y > _wanderLocation.y + 5)) {
      // Moving Either Up or Down
      if (pos.y < _wanderLocation.y) {
          if (getVelocity().y < _maxVelocity.y)
              impulse({ 0, (float)(dt * _groundspeed) });
      }
      else if (pos.y > _wanderLocation.y){
          if (getVelocity().y > -_maxVelocity.y)
              impulse({ 0,  -(float)(dt * _groundspeed)});
      }
  }
  else {
      // Dampen Y axis movement
      dampen({ 1.0f, 0.9f });
  }

  //Once we arrive at the destination, do nothing for a few seconds
  if (_startTime)
  {
      _start = std::chrono::system_clock::now();
      _startTime = false;
      _tPause = true;
  }

  //Get current system time every frame
  std::chrono::time_point<std::chrono::system_clock> dur = std::chrono::system_clock::now();

  //Find difference between dur and _start
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(dur - _start).count();
  
  //If 5 seconds have passed since we got _start
  if (seconds == 5)
  {
      if (!_locationChosen)
      {
          //Choose new location to go to
          PickWanderLocation();
        
          _locationChosen = true;
          _tPause = false;
      }
  }

  //Once we arrive at the destination, start timer
  if ((pos.y > _wanderLocation.y - 5 && pos.y < _wanderLocation.y + 5) || (pos.x > _wanderLocation.x - 5 && pos.x < _wanderLocation.x + 5))
  {
      if (!_tPause)
      {
          _startTime = true;
          _locationChosen = false;
      }
  }

  /*
  //Are we in air?
  if (!_grounded) {
    // Check to see if we have landed yet
    _grounded = isGrounded();
    // disable friction while jumping
    setFriction(0.1f);
  } else {
    setFriction(0.1f);
  }
  */

  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}

void CatAI::PickWanderLocation()
{
    //Choose random empty tile on map as target location
    auto empty = ls::findTiles(ls::EMPTY);

    int randomIndex = rand() % empty.size();

    _wanderLocation =ls::getTilePosition(empty[randomIndex]) + Vector2f(20.f, 20.f);

    std::cout << randomIndex << std::endl;

}

CatAI::CatAI(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size) {
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(50.f, 50.f);
  _groundspeed = 5.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
  ls::loadLevelFile("res/levels/gameScene.txt", 40.0f);
}
