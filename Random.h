//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
	//Takes an integer range and returns a random integer
	//in that range
	static int RandomInt(int low, int high)
	{
		return low + rand() % ((high + 1) - low);
	}

	//Take an integer range, converts that integer and 
	//returns a float
	static float RandomFloat(int low, int high)
	{
		low *= 100;
		high *= 100;
		int result = (low + rand() % ((high + 1) - low));
		float temp = (float)result;
		return temp / 100;
	}
};

#endif //RANDOM_H