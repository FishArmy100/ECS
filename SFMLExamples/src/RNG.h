#pragma once
#include <random>

class RNG
{
public:
	RNG(size_t seed = std::random_device()()) : m_Seed(seed), engine(seed) {}

	float Range(float a, float b)
	{
		auto dist = std::uniform_real_distribution<float>(a, b);
		return dist(engine);
	}

	int Range(int a, int b)
	{
		auto dist = std::uniform_int_distribution<int>(a, b);
		return dist(engine);
	}

private:
	size_t m_Seed;
	std::mt19937 engine;
};