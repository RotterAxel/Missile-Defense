//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#include "..\\2DEngine\Rect.h"

Rect::Rect()
{
}

Rect::Rect(const Vec2& a, const Vec2& b)
	: minPt(a), maxPt(b)
{
}

Rect::Rect(float x0, float y0, float x1, float y1)
	: minPt(x0, y0), maxPt(x1, y1)
{
}

//Method that takes a circle and forces the rdius inside
//a rectangle
void Rect::forceInside(Circle& A)
{
	Vec2 p = A.c;
	float r = A.r;

	// Modify coordinates to force inside.
	if (p.x - r < minPt.x)
		p.x = minPt.x + r;
	if (p.x + r > maxPt.x)
		p.x = maxPt.x - r;

	if (p.y - r < minPt.y)
		p.y = minPt.y + r;
	if (p.y + r > maxPt.y)
		p.y = maxPt.y - r;

	// Save forced position.
	A.c = p;
}

//Method that checks if a point is inside a rectangle
bool Rect::isPtInside(const Vec2& pt)
{
	return pt.x >= minPt.x && pt.y >= minPt.y &&
		pt.x <= maxPt.x && pt.y <= maxPt.y;
}