//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#include "Detonation.h"

//Constructor that takes a coordinate
Detonation::Detonation(Vec2 detPoint)
{
	detonationPoint = detPoint;
	animationFrame = 0;
	timeToNextFrame = 0.0f;
}
