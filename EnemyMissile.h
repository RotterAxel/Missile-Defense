//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef ENEMY_MISSILE_H
#define ENEMY_MISSILE_H

#include "Vec2.h"
#include <Windows.h>
#include "Random.h"



class EnemyMissile
{
public:
	//Default constructor
	EnemyMissile();

	//Method that updates the position of a missile
	Vec2	UpdatePosition(float time);

	//Method that checks if a Missile has detonated.
	bool	CheckDetonation();

	//Getter for data members
	Vec2	GetCurrPos();
	Vec2	GetEndPos();
	
private:
	//Calculate the direction a missile should go with the 
	//starting and ending positions set in the constructor.
	Vec2	CalcDirection(Vec2 EndPos, Vec2 StartPos);

	//Data members
	Vec2	mStartPos;
	Vec2	mEndPos;
	Vec2	mCurrPos;
	Vec2	mDirection;
};

#endif //ENEMY_MISSILE_H

