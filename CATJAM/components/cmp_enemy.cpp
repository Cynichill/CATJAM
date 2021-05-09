#include "cmp_enemy.h"
#include "altSystemPhysics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

bool Enemy::isGrounded() const {
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

void Enemy::update(double dt) {

    const auto pos = _parent->getPosition();

    //Teleport to start if we fall off map.
    if (pos.y > ls::getHeight() * ls::getTileSize()) {
        deleteMe = true;
    }

    // Clamp velocity.
    auto v = getVelocity();
    v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
    v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
    setVelocity(v);

    PhysicsComponent::update(dt);
}

bool Enemy::GetDelete() { return deleteMe; }

Enemy::Enemy(Entity* p,
    const Vector2f& size, sf::Vector2f maxVelocity)
    : PhysicsComponent(p, true, size) {
    _size = sv2_to_bv2(size, true);
    _maxVelocity = maxVelocity;
    _groundspeed = 5.f;
    _grounded = false;
    _body->SetSleepingAllowed(false);
    _body->SetFixedRotation(true);
    //Bullet items have higher-res collision detection
    _body->SetBullet(true);
    ls::loadLevelFile("res/levels/gameScene.txt", 40.0f);
}
