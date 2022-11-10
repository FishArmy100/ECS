#pragma once
#include "SFML/Graphics.hpp"

using Vec2 = sf::Vector2f;
using Rect = sf::Rect<float>;

enum class ShapeType
{
    Circle,
    Square,
};

struct Transform
{
    Vec2 Pos;
    float Scale;

    Transform(Vec2 pos, float scale) :
        Pos(pos), Scale(scale) {}
};

struct Renderer
{
    sf::Color Color;
    ShapeType Shape;

    Renderer(sf::Color color, ShapeType shape) :
        Color(color), Shape(shape) {}
};

struct Camera
{
    Vec2 Size;
    Camera(Vec2 size) : Size(size) {}
};