#include <iostream>
#include "SFML/Graphics.hpp"
#include "EntityRegistry.h"
#include "SFMLRenderer.h"
#include "BoidController.h"
#include <chrono>

using namespace Examples;
using namespace ECS;
using namespace ECS::Utils;

std::chrono::microseconds CurrentTime()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

void RenderText(Vec2 screenPos, const std::string& text, sf::RenderWindow& window, const sf::Font& font)
{
    //Declare a Text object
    sf::Text textVisual(text, font);

    //Set character size
    textVisual.setCharacterSize(30);
    textVisual.setPosition(screenPos);

    //Set fill color
    textVisual.setFillColor(Color::White);
    window.draw(textVisual);
}

int main()
{
    EntityRegistry<Transform, Renderer, Boid, AvoidWalls, Seperation, Alignment, Coherence> registry = {};
    sf::RenderWindow window(sf::VideoMode(1500, 900), "SFML works!");
    auto renderer = SFMLRenderer(Ref(window), Ref(registry));

    sf::View view = { sf::Vector2f(0, 0), window.getDefaultView().getSize() };
    auto time = std::chrono::high_resolution_clock::now();
    window.setView(view);
    Vec2 windowSize = view.getSize();

    //Declare a Font object
    sf::Font font;

    //Load and check the availability of the font file
    if (!font.loadFromFile("C:\\dev\\ECS\\SFMLExamples\\vendor\\ARIAL.TTF"))
    {
        std::cout << "can't load font" << std::endl;
    }

    BoidSpawnData spawnData =
    {
        20.0f,
        Boid(Color::Red, {1.0f, 0}, 200.0f, 50.0f),
        AvoidWalls(1.0f),
        Alignment(0.1f),
        Coherence(0.5f, 2),
        Seperation(0.5f)
    };

    auto controller = BoidController(registry, view.getCenter(), view.getSize(), 20.0f);
    controller.SpawnBoids(100, spawnData);

    while (window.isOpen())
    {
        window.clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        auto newTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = newTime - time;
        time = newTime;

        controller.Update(delta.count());
        renderer.Render();

        std::stringstream ss;
        ss << std::setprecision(4) << "frame time: " << (delta.count() * 1000) << "ms";
        RenderText({-windowSize.x / 2, -windowSize.y / 2 }, ss.str(), window, font);
        window.display();
    }

    return 0;
}
