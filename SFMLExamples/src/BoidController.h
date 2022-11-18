#pragma once
#include "EntityRegistry.h"
#include "Components.h"
#include "RNG.h"
#include "BoidSpawnData.h"

template<typename... TComponents>
class BoidController
{
	using Registry = ECS::EntityRegistry<TComponents...>;
	template<typename T>
	using Ref = ECS::Utils::Ref<T>;

public:
	static constexpr double PI = 3.14159265358979323846;

public:
	BoidController(Registry& registry, Vec2 center, Vec2 size, float margin) : 
		m_Registry(registry), m_Center(center), m_Size(size), m_Margin(margin) {}

	void Update(float timeStep)
	{
		KeepInBounds(timeStep);
		AvoidOthers(timeStep);
		AlignWithOthers(timeStep);
		GroupTogether(timeStep);

		UpdatePos(timeStep);
	}

	void SpawnBoids(size_t count, const BoidSpawnData& boidSpawnData)
	{
		RNG rng{};

		for (size_t i = 0; i < count; i++)
		{
			float x = rng.Range(-m_Size.x / 2 + m_Margin, m_Size.x / 2 - m_Margin) + m_Center.x;
			float y = rng.Range(-m_Size.y / 2 + m_Margin, m_Size.y / 2 - m_Margin) + m_Center.y;

			Vec2 startVelocity = { rng.Range(-1.0f, 1.0f), rng.Range(-1.0f, 1.0f) };

			EntityId e = m_Registry->CreateEntity();
			m_Registry->AddComponent<Transform>(e, Transform({ x, y }, 0, boidSpawnData.Size));
			m_Registry->AddComponent<Boid>(e, boidSpawnData.BoidData);
			m_Registry->AddComponent<AvoidWalls>(e, boidSpawnData.AvoidWallsData);
			m_Registry->AddComponent<Seperation>(e, boidSpawnData.SeperationData);
			m_Registry->AddComponent<Alignment>(e, boidSpawnData.AlighmentData);
			m_Registry->AddComponent<Coherence>(e, boidSpawnData.CoherenceData);

			m_Registry->GetComponent<Boid>(e).Velocity += startVelocity; // ???
		}
	}

	void UpdatePos(float delta)
	{
		auto view = m_Registry->GetView<Boid, Transform>();
		for (auto [boid, transform] : view)
		{
			Vec2 normalized = NormalizeVec(boid->Velocity);
			transform->Pos += normalized * boid->DesiredVelocity * delta;

			transform->Rot = -atan2f(normalized.y, normalized.x) / (2 * PI) * 360 + 90;
		}
	}

	void GroupTogether(float delta)
	{
		auto selfView = m_Registry->GetView<Transform, Boid, Coherence>();
		auto coherenceView = m_Registry->GetView<Transform>();
		for (auto [transform, boid, coherence] : selfView)
		{
			Vec2 avgPos = {};
			int neighborCount = 0;

			for (auto [otherTransform] : coherenceView)
			{
				if (otherTransform == transform || SquareDistance(transform->Pos, otherTransform->Pos) > boid->Vision * boid->Vision * coherence->VisionMultiplier)
					continue;

				avgPos += otherTransform->Pos;
				neighborCount++;
			}

			if (neighborCount > 0)
			{
				avgPos = avgPos / (float)neighborCount;
				Vec2 dir = NormalizeVec(avgPos - transform->Pos); 
				boid->Velocity += coherence->Value * dir * delta; 
			}
		}
	}

	void AlignWithOthers(float delta)
	{
		auto selfView = m_Registry->GetView<Transform, Boid, Alignment>();
		auto alignmentView = m_Registry->GetView<Transform, Boid>();
		for (auto [transform, boid, alignment] : selfView)
		{
			Vec2 avgVel = {};
			int neighborCount = 0;

			for (auto [otherTransform, otherBoid] : alignmentView)
			{
				if (otherTransform == transform || SquareDistance(transform->Pos, otherTransform->Pos) > boid->Vision * boid->Vision)
					continue;

				neighborCount++;
				avgVel += otherBoid->Velocity;
			}

			if (neighborCount > 0)
				boid->Velocity += alignment->Value * NormalizeVec(avgVel) * delta;
		}
	}

	void AvoidOthers(float delta)
	{
		auto selfView = m_Registry->GetView<Transform, Boid, Seperation>();
		auto avoidenceView = m_Registry->GetView<Transform>();
		for (auto [transform, boid, seperation] : selfView)
		{
			for (auto [other] : avoidenceView)
			{
				if (other == transform || SquareDistance(transform->Pos, other->Pos) > boid->Vision * boid->Vision)
					continue;

				Vec2 movement = {};

				movement.x += transform->Pos.x - other->Pos.x;
				movement.y += transform->Pos.y - other->Pos.y;

				movement = NormalizeVec(movement);
				boid->Velocity += seperation->Value * delta * movement;
			}
		}
	}

	void KeepInBounds(float delta)
	{
		auto view = m_Registry->GetView<AvoidWalls, Transform, Boid>();
		for (auto [avoidWalls, transform, boid] : view)
		{
			Vec2 maxDist = { m_Size.x / 2 - m_Margin, m_Size.y / 2  - m_Margin };
			Vec2 dist = transform->Pos - m_Center;

			float val = avoidWalls->Value * delta;

			if (dist.x < -maxDist.x) {
				boid->Velocity.x += val;
			}
			if (dist.x > maxDist.x) {
				boid->Velocity.x -= val;
			}
			if (dist.y < -maxDist.y) {
				boid->Velocity.y += val;
			}
			if (dist.y > maxDist.y) {
				boid->Velocity.y -= val;
			}
		}
	}

	float SquareDistance(Vec2 a, Vec2 b)
	{
		return powf(a.x - b.x, 2) + powf(a.y - b.y, 2);
	}

	Vec2 NormalizeVec(Vec2 vec)
	{
		float mag = sqrtf(vec.x * vec.x + vec.y * vec.y);
		return vec / mag;
	}

private:
	Ref<Registry> m_Registry;
	Vec2 m_Center;
	Vec2 m_Size;
	float m_Margin;
};