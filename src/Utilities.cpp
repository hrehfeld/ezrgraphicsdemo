#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utilities.h"
#include "MyMath.h"
#include <cmath>

namespace Ezr
{
	std::string Utilities::loadFile(std::string filename)
	{
		std::ifstream file(filename.c_str());
		if (!file) {
			std::cerr << "Couldn't open file " << filename << std::endl;
			return "";
		}
		std::stringstream result;
		result << file.rdbuf();
		return result.str();
		
	// std::ifstream file(filename.c_str());

	// 	//make sure we opened the file correctly
	// 	if (!file)
	// 	{
	// 		std::cerr << "Could not open file: " + filename << std::endl;
	// 		return "";
	// 	}

	// 	std::string line = "";
	// 	std::string result = "";

	// 	while(getline(file, line))
	// 	{
	// 		result = result + "\n" + line;
	// 	}

	// 	std::cout << result << std::endl;

	// 	file.close();
	// 	return result;
	}

    float Utilities::nearPlaneSize(float nearPlaneDist, float fov)
    {
        float halfFov = fov / 2.0f;
        return nearPlaneDist * tan(halfFov / 360.0f * 2.0f * MyMath::PI);
    }
}
