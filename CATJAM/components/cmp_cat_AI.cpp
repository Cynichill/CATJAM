#include "cmp_cat_AI.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

bool timeCheck = false;

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

        if (!_minigameVer)
        {

            //If not close to target location, move towards it on the correct axis
            if ((pos.x < _target.x - 5 && pos.x < _target.x + 5) || (pos.x > _target.x - 5 && pos.x > _target.x + 5)) {

                // Moving Either Left or Right
                if (pos.x < _target.x) {
                    if (getVelocity().x < _maxVelocity.x)
                        impulse({ (float)(dt * _groundspeed), 0 });
                }
                else if (pos.x > _target.x) {
                    if (getVelocity().x > -_maxVelocity.x)
                        impulse({ -(float)(dt * _groundspeed), 0 });
                }
            }
            else {
                // Dampen X axis movement
                dampen({ 0.9f, 1.0f });
            }

            if ((pos.y < _target.y - 5 && pos.y < _target.y + 5) || (pos.y > _target.y - 5 && pos.y > _target.y + 5)) {
                // Moving Either Up or Down
                if (pos.y < _target.y) {
                    if (getVelocity().y < _maxVelocity.y)
                        impulse({ 0, (float)(dt * _groundspeed) });
                }
                else if (pos.y > _target.y) {
                    if (getVelocity().y > -_maxVelocity.y)
                        impulse({ 0,  -(float)(dt * _groundspeed) });
                }
            }
            else {
                // Dampen Y axis movement
                dampen({ 1.0f, 0.9f });
            }

            int range = 60;

            //Once we arrive at the destination, start timer
            if ((pos.y > _target.y - range && pos.y < _target.y + range) && (pos.x > _target.x - range && pos.x < _target.x + range))
            {
                if (!_startTime && GetChosen() == true)
                {
                    _start = std::chrono::system_clock::now();
                    _startTime = true;
                }

                if (_startTime)
                {
                    //Get current system time every frame
                    std::chrono::time_point<std::chrono::system_clock> dur = std::chrono::system_clock::now();

                    //Find difference between dur and _start
                    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(dur - _start).count();

                    //If 5 seconds have passed since we got to the location
                    if (seconds == 5)
                    {
                        _tPause = true;
                        SetChosen(false);
                        _startTime = false;
                    }
                }
            }
        }
        else
        {
            //If not close to target location, move towards it on the correct axis
            if ((pos.x < _target.x - 5 && pos.x < _target.x + 5) || (pos.x > _target.x - 5 && pos.x > _target.x + 5)) {

                // Moving Either Left or Right
                if (pos.x < _target.x) {
                    if (getVelocity().x < _maxVelocity.x)
                        impulse({ (float)(dt * _groundspeed), 0 });
                }
                else if (pos.x > _target.x) {
                    if (getVelocity().x > -_maxVelocity.x)
                        impulse({ -(float)(dt * _groundspeed), 0 });
                }
            }
            else {
                // Dampen X axis movement
                dampen({ 0.9f, 1.0f });
            }
        }
  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}

void CatAI::PickTarget(std::string targ, Vector2f& targLocation)
{

    if (targ == "WANDER")
    {
        //Choose random empty tile on map as target location
        auto empty = ls::findTiles(ls::EMPTY);

        int randomIndex = rand() % empty.size();

        _target = ls::getTilePosition(empty[randomIndex]) + Vector2f(20.f, 20.f);
    }
    else if (targ == "HUNGRY" || targ == "DIRTY" || targ == "MOUSE")
    {
        _target = targLocation;
    }
    

    _tPause = false;
}

sf::Vector2f CatAI::GetTarget() { return _target; }

bool CatAI::GetPause() { return _tPause; }

void CatAI::SetChosen(bool chos)
{
    _locationChosen = chos;
}

bool CatAI::GetChosen() { return _locationChosen; }

CatAI::CatAI(Entity* p,
                                               const Vector2f& size, bool minigameVer, sf::Vector2f maxVelocity)
    : PhysicsComponent(p, true, size) {
  _size = sv2_to_bv2(size, true);
  _maxVelocity = maxVelocity;
  _groundspeed = 5.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
  _minigameVer = minigameVer;
  ls::loadLevelFile("res/levels/gameScene.txt", 40.0f);
}
