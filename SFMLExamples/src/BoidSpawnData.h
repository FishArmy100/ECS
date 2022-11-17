#pragma once
#include "Components.h"

struct BoidSpawnData
{
	float Size;
	Boid BoidData;
	AvoidWalls AvoidWallsData;
	Alignment AlighmentData;
	Coherence CoherenceData;
	Seperation SeperationData;

	BoidSpawnData(float size, const Boid& boidData, const AvoidWalls& avoidWalls, const Alignment& alignment, const Coherence& coherence, const Seperation& seperation) :
		Size(size), BoidData(boidData), AvoidWallsData(avoidWalls),
		AlighmentData(alignment), CoherenceData(coherence),
		SeperationData(seperation) 
	{}
};