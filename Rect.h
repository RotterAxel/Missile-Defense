//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef RECT_H
#define RECT_H

#include "Circle.h"

class Rect
{
public:
	//Cosntructors
	Rect();
	Rect(const Vec2& a, const Vec2& b);
	Rect(float x0, float y0, float x1, float y1);

	//Method that takes a circle and forces the rdius inside
	//a rectangle
	void forceInside(Circle& A);

	//Method that checks if a point is inside a rectangle
	bool isPtInside(const Vec2& pt);

	Vec2 minPt;
	Vec2 maxPt;
};

#endif // RECT_H