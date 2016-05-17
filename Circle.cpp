//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#include "..\\2DEngine\Circle.h"

//Default constructor
Circle::Circle()
	: r(0.0f), c(0.0f, 0.0f)
{
}

//Constructor that take a center coordinate and a radius
Circle::Circle(float R, const Vec2& center)
	: r(R), c(center)
{
}

//Logic that calculates if a circle is colliding with 
//another. Take anothe circle and a normal.
bool Circle::hits(Circle& A, Vec2& normal)
{
	Vec2 u = A.c - c;

	if (u.length() <= r + A.r)
	{
		normal = u.normalize();

		// Make sure circles never overlap--at most
		// they can be tangent.
		A.c = c + (normal * (r + A.r));

		return true;
	}
	return false;
}

//Logic that calculates if a point is inside the circle. Take the coordinate
//of a point
bool Circle::isInside(Vec2& B)
{
	if ((B.x - c.x) * (B.x - c.x) + (B.y - c.y) * (B.y - c.y) <= r*r)
	{
		return true;
	}
	else
	{
		return false;
	}
}