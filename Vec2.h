//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef VEC2_H
#define VEC2_H

#include <Windows.h>
#include <math.h>

class Vec2
{
public:
	//Constructors
	Vec2();
	Vec2(float s, float t);
	Vec2(float d[2]);
	Vec2(const POINT& p);

	//Methods
	Vec2 operator+(const Vec2& rhs)const;
	Vec2 operator-(const Vec2& rhs)const;
	Vec2 operator-();

	operator POINT();

	void operator+=(const Vec2& rhs);
	void operator-=(const Vec2& rhs);
	void operator*=(float s);
	void operator/=(float s);

	float length();
	Vec2& normalize();

	float dot(const Vec2& rhs);

	Vec2& reflect(const Vec2& normal);

	// Data members.
	float x;
	float y;
};

// Define with left hand and right hand sides reversed
// so we can write both v*s and s*v.
Vec2 operator*(const Vec2& v, float s);
Vec2 operator*(float s, const Vec2& v);
Vec2 operator/(const Vec2& v, float s);
Vec2 operator/(float s, const Vec2& v);

#endif // VEC2_H