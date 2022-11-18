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
    float Rot;
    float Scale;

    Transform(Vec2 pos, float rot, float scale) :
        Pos(pos), Rot(rot), Scale(scale) {}
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
    float DesiredVelocity;
    float Vision;
    Boid(Color color, Vec2 velocity, float desiredVelocity, float vision) : 
        BoidColor(color), Velocity(velocity), 
        DesiredVelocity(desiredVelocity), Vision(vision) {}

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
    float Value;
    float VisionMultiplier;
    Coherence(float value, float visionMultiplier) : Value(value), VisionMultiplier(visionMultiplier) {}
};

struct AvoidWalls
{
    float Value;
    AvoidWalls(float value) : Value(value) {}
};

struct Seperation
{
    float Value;
    Seperation(float value) : Value(value) {}
};

struct Alignment
{
    float Value;
    Alignment(float value) : Value(value) {}
};