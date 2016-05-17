//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef SPAWN_TIMER_H
#define SPAWN_TIMER_H

#include "Random.h"

class SpawnTimer
{

public:
	//Default constructor
	SpawnTimer() :spawnTime(0.0f) {}

	void UpdateSpawnTime(float time)
	{
		spawnTime -= time;	
	}

	bool Spawn()
	{
		if (spawnTime <= 0)
		{
			spawnTime = Random::RandomFloat(3, 5);
			return true;
		}
		else
			return false;
	}

private:
	float spawnTime;
};

#endif //SPAWN_TIMER_H