#include <iostream>
#include "BitSet.h"
#include "Tuple.h"
#include "Optional.h"
#include "Array.h"
#include "List.h"
#include "EntityRegistry.h"
#include "SFML/Graphics.hpp"

using namespace ECS;
using namespace ECS::Utils;

struct Pos
{
	float X = 0;
	float Y = 0;
	float Z = 0;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
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
}