#pragma once
#include "SFML/Graphics.hpp"

using Vec2 = sf::Vector2f;
using Rect = sf::Rect<float>;
using Color = sf::Color;
using Rect = sf::FloatRect;

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

struct Boid
{
    Color BoidColor;
    Vec2 Velocity;
    float MaxVelocity;
    Boid(Color color, Vec2 velocity, float maxVelocity) : BoidColor(color), Velocity(velocity), MaxVelocity(maxVelocity) {}

    static sf::ConvexShape GetShape(float length, float width)
    {
        sf::ConvexShape shape = sf::ConvexShape(3);
        shape.setPoint(0, { 0, -length / 2 });
        shape.setPoint(1, { width / 2, length / 2 });
        shape.setPoint(2, { -width / 2, length / 2 });
        return shape;
    }
};

struct Coherence
{
    const float Value;
    Coherence(float value) : Value(value) {}
};

struct Seperation
{
    const float Value;
    Seperation(float value) : Value(value) {}
};

struct Alignment
{
    const float Value;
    Alignment(float value) : Value(value) {}
};