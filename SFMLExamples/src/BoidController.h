#pragma once
#include "EntityRegistry.h"
#include "Components.h"
#include "RNG.h"

template<typename... TComponents>
class BoidController
{
	using Registry = ECS::EntityRegistry<TComponents...>;
	template<typename T>
	using Ref = ECS::Utils::Ref<T>;

public:
	BoidController(Registry& registry, Vec2 center, Vec2 size) : 
		m_Registry(registry), m_Center(center), m_Size(size) {}

	void Update(float timeStep)
	{

	}

	void SpawnBoids(size_t count)
	{
		RNG rng{};

		for (size_t i = 0; i < count; i++)
		{
			float x = rng.Range(-m_Size.x / 2, m_Size.x / 2) + m_Center.x;
			float y = rng.Range(-m_Size.y / 2, m_Size.y / 2) + m_Center.y;

			std::cout << x << ", " << y << "\n";

			EntityId e = m_Registry->CreateEntity();
			m_Registry->AddComponent<Transform>(e, Transform({ x, y }, 100.0f));
			m_Registry->AddComponent<Boid>(e, Boid(Color::Red, { 0, 0 }, 0));
		}
	}

private:
	Ref<Registry> m_Registry;
	Vec2 m_Center;
	Vec2 m_Size;
};
