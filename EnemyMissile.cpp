#include "EnemyMissile.h"

//Default constructor
EnemyMissile::EnemyMissile()
{
	mEndPos.x = 432;
	mEndPos.y = 6912;
	mStartPos.x = Random::RandomInt(0, 864);
	mStartPos.y = 0;
	mCurrPos = mStartPos;
	mDirection = CalcDirection(mStartPos, mEndPos);
}

//Method that updates the position of a missile
Vec2 EnemyMissile::UpdatePosition(float time)
{
	mCurrPos += mDirection * time;
	return mCurrPos;
}

Vec2 EnemyMissile::GetCurrPos()
{
	return mCurrPos;
}

Vec2 EnemyMissile::GetEndPos()
{
	return mEndPos;
}

//Method that checks if a Missile has detonated.
bool EnemyMissile::CheckDetonation()
{
	if (mCurrPos.y > 500)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Calculate the direction a missile should go with the 
//starting and ending positions set in the constructor.
Vec2 EnemyMissile::CalcDirection(Vec2 StartPos, Vec2 EndPos)
{
	Vec2 tempDir;
	tempDir.x = EndPos.x - StartPos.x;
	tempDir.y = EndPos.y - StartPos.y;

	tempDir.normalize();
	tempDir *= 50;

	return tempDir;
}

