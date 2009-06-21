#ifndef _MYMATH_H_
#define _MYMATH_H_
#include <algorithm>

#include <iostream>

class MyMath
{
public:
	static const float PI;

	/**
	 * restrict value to [min, max]
	 */
	static float clamp(const float value, const float min, const float max)
	{ 
		return std::min(max, std::max(min, value)); 
	}

	/**
	 * restrict value to [min, max], but if value > max, return min + (value - max)
	 */
	static float clampLoop(float value, const float min, const float max);
			
private:
    MyMath();
    virtual ~MyMath();	
};

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

#endif /* _MYMATH_H_ */
