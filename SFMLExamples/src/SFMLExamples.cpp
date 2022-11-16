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
    EntityRegistry<Transform, Renderer, Boid> registry = {};
    sf::RenderWindow window(sf::VideoMode(860, 500), "SFML works!");
    auto renderer = SFMLRenderer(Ref(window), Ref(registry));

    sf::View view = { sf::Vector2f(0, 0), window.getDefaultView().getSize() };
    auto time = std::chrono::high_resolution_clock::now();

    auto controller = BoidController(registry, view.getCenter(), view.getSize());
    controller.SpawnBoids(20);

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

        std::cout << delta << "\n";

        renderer.Render(view);
    }

    return 0;
}
