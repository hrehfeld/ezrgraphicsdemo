#include "MyMath.h"

const float MyMath::PI = 3.1415926535897932384626433832795;

float MyMath::clampLoop(float value, const float min, const float max)
{
	float range = max - min;
	
	while (value > max)
	{
		std::cout << "clampLoop: subtracting " << std::endl;
		value -= range;
	}
	
	while (value < min)
	{
		std::cout << "clampLoop: adding " << std::endl;
		value += range;
	}

	// std::cout << "clampLoop: end " << std::endl;
	return value;
}

