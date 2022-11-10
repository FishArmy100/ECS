#include <iostream>
#include "SFML/Graphics.hpp"
#include "EntityRegistry.h"
#include "SFMLRenderer.h"

using namespace Examples;

int main()
{
    ECS::EntityRegistry<Transform, Renderer> registry = {};
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    auto renderer = SFMLRenderer(window, registry);


    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
