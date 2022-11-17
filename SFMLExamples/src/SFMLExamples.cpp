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

int main()
{
    EntityRegistry<Transform, Renderer, Boid, AvoidWalls, Seperation, Alignment, Coherence> registry = {};
    sf::RenderWindow window(sf::VideoMode(1500, 900), "SFML works!");
    auto renderer = SFMLRenderer(Ref(window), Ref(registry));

    sf::View view = { sf::Vector2f(0, 0), window.getDefaultView().getSize() };
    auto time = std::chrono::high_resolution_clock::now();

    BoidSpawnData spawnData =
    {
        20.0f,
        Boid(Color::Red, {0, 20.0f}, 60.0f, 20.0f),
        AvoidWalls(1000.0f),
        Alignment(10.0f),
        Coherence(30.0f),
        Seperation(10.0f)
    };

    auto controller = BoidController(registry, view.getCenter(), view.getSize(), 20.0f);
    controller.SpawnBoids(100, spawnData);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        auto newTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = newTime - time;
        time = newTime;

        controller.Update(delta.count() * 3);
        renderer.Render(view);
    }

    return 0;
}
