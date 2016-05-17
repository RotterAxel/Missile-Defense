//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#include "PlayerMissile.h"

PlayerMissile::PlayerMissile(Vec2 EndPosition)
	:EndPos(EndPosition)
{
	StartPos.x = 432;
	StartPos.y = 498;
	CurrPos = StartPos;
	Direction = CalcDirection(StartPos, EndPos);
}

//Method that checks if a Missile has detonated.
bool PlayerMissile::CheckDetonation()
{
	if (CurrPos.y < EndPos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//Method that updates the position of a missile
Vec2 PlayerMissile::UpdatePosition(float time)
{
	CurrPos += Direction * time;
	return CurrPos;
}

Vec2 PlayerMissile::GetCurrPos()
{
	return CurrPos;
}

Vec2 PlayerMissile::GetEndPos()
{
	return EndPos;
}

//Calculate the direction a missile should go with the 
//starting and ending positions set in the constructor.
Vec2 PlayerMissile::CalcDirection(Vec2 StartPos, Vec2 EndPos)
{
	Vec2 tempDir;
	tempDir.x = EndPos.x - StartPos.x;
	tempDir.y = EndPos.y - StartPos.y;

	tempDir.normalize();
	tempDir *= 300;

	return tempDir;
}
