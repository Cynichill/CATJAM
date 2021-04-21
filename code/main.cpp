#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    window.setFramerateLimit(60);

    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();

        menu.draw(window);
        menu.MouseCheck(window);

        window.display();
    }
    return 0;
}