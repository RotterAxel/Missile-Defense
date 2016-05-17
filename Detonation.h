//Programmer: Axel Rotter Morgado
//Date: 18.04.2016 

#ifndef DETONATION_H
#define DETONATION_H

#include "Sprite.h"
#include "Vec2.h"

class Detonation
{
public:
	//Constructor that takes a coordinate
	Detonation(Vec2 detonationPoint);

	//Data members
	Vec2		detonationPoint;
	const float	frameTime = 0.05f;
	int			animationFrame;
	float		timeToNextFrame;
};

#endif //DETONATION_H
