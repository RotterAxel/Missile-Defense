//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef CIRCLE_H
#define CIRCLE_H

#include "Vec2.h"

class Circle
{
public:
	//Default constructor
	Circle();
	//Constructor that defines the radius and center of the 
	//circle
	Circle(float R, const Vec2& center);

	//Logic that calculates if a circle is colliding with 
	//another.
	bool hits(Circle& A, Vec2& normal);

	//Logic that calculates if a point is inside the circle.
	bool isInside(Vec2& B);

	//Data members
	float r; // radius
	Vec2  c; // center point
};
#endif // CIRCLE_H