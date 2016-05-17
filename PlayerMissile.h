//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef PLAYER_MISSILE_H
#define PLAYER_MISSILE_H

#include "Vec2.h"
#include <Windows.h>


class PlayerMissile
{
public:
	//Constructor that takes a coordinate 
	PlayerMissile(Vec2 EndPosition);

	//Method that checks if a Missile has detonated.
	bool CheckDetonation();

	//Method that updates the position of a missile
	Vec2 UpdatePosition(float time);

	//Getter for data members
	Vec2 GetCurrPos();
	Vec2 GetEndPos();

private:
	//Calculate the direction a missile should go with the 
	//starting and ending positions set in the constructor.
	Vec2 CalcDirection(Vec2 EndPos, Vec2 StartPos);

	//Data members
	Vec2 StartPos;
	Vec2 EndPos;
	Vec2 CurrPos;
	Vec2 Direction;
};

#endif //PLAYERMISSILE_H
