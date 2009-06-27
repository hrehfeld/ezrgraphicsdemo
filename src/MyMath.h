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

#endif /* _MYMATH_H_ */
