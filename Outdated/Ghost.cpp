//Ghost.cpp
#include "Ghost.h"
#include "system_renderer.h"

using namespace sf;
using namespace std;

int i = 301;
int iy = 0;
int ix = 0;

void Ghost::Update(double dt) {

    int random = 0;

    random = rand() % 4;

    if (i > 300)
    {
        i = 0;
        if (random == 0)
        {
            ix = 0;
            iy = -1;
        }
        else if (random == 1)
        {
            ix = 0;
            iy = 1;
        }
        else if (random == 2)
        {
            ix = 1;
            iy = 0;
        }
        else if (random == 3)
        {
            ix = -1;
            iy = 0;
        }
    }
    i++;

    move(sf::Vector2f(ix * _speed * dt, iy * _speed * dt));

    Entity::Update(dt);
}

Ghost::Ghost()
    : _speed(200.0f), Entity(make_unique<CircleShape>(25.f)) {
    _shape->setFillColor(Color::Red);
    _shape->setOrigin(Vector2f(25.f, 25.f));
}

void Ghost::Render() const {
    Renderer::queue(_shape.get());
}