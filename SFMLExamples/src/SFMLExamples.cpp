#include <iostream>
#include "SFML/Graphics.hpp"
#include "EntityRegistry.h"
#include "glm/vec2.hpp"

enum class ShapeType
{
    Circle,
    Square,
};

struct Transform
{
    glm::vec2 Pos;

    Transform(const glm::vec2& pos) : 
        Pos(pos) {}
};

struct Renderer
{
    sf::Color Color;
    ShapeType Shape;

    Renderer(sf::Color color, ShapeType shape) : 
        Color(color), Shape(shape) {}
};

int main()
{
    ECS::EntityRegistry<Transform, Renderer> registry = {};

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

    return 0;
}
