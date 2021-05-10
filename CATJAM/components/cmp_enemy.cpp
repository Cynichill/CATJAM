#include "cmp_enemy.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

void Enemy::update(double dt) {

    const auto pos = _parent->getPosition();

    //If enemy hits a wall or despawn tile, set for delete
    if (ls::getTileAt(pos) == ls::DESPAWN || ls::getTileAt(pos) == ls::WALL)
    {
        deleteMe = true;
    }
    // Clamp velocity.
    auto v = getVelocity();
    v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
    v.y = copysign(min(abs(_airspeed), _maxVelocity.y), v.y);
    setVelocity(v);

    PhysicsComponent::update(dt);
}

bool Enemy::GetDelete() { return deleteMe; }

void Enemy::SetDelete(bool del)
{
    deleteMe = del;
}

Enemy::Enemy(Entity* p,
    const Vector2f& size, sf::Vector2f maxVelocity)
    : PhysicsComponent(p, true, size) {
    _size = sv2_to_bv2(size, true);
    _maxVelocity = maxVelocity;
    _airspeed = 100.f;
    _body->SetSleepingAllowed(false);
    _body->SetFixedRotation(true);
    //Bullet items have higher-res collision detection
    _body->SetBullet(true);
}
