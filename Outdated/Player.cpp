//player.cpp
#include "player.h"
#include "system_renderer.h"
using namespace sf;
using namespace std;


void Player::Update(double dt) {

    
    //Move in four directions based on keys
    const Keyboard::Key controls[4] = {
    Keyboard::W,   // UP
    Keyboard::S,   // Down
    Keyboard::A,  // Left
    Keyboard::D, // Right
    };

    int iy = 0;
    int ix = 0;

    if (Keyboard::isKeyPressed(controls[0]))
    {
        iy = -1;
    }
    else if (Keyboard::isKeyPressed(controls[1]))
    {
        iy = 1;
    }

    if (Keyboard::isKeyPressed(controls[2]))
    {
        ix = -1;
    }
    else if (Keyboard::isKeyPressed(controls[3]))
    {
        ix = 1;
    }

    move(sf::Vector2f(ix * _speed * dt, iy * _speed * dt));
 
    Entity::Update(dt);
}

Player::Player()
    : _speed(200.0f), Entity(make_unique<CircleShape>(25.f)) {
    _shape->setFillColor(Color::Yellow);
    _shape->setOrigin(Vector2f(25.f, 25.f));
}

void Player::Render() const {
    Renderer::queue(_shape.get());
}