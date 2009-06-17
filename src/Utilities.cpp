#include <fstream>
#include <iostream>
#include "Utilities.h"

namespace Ezr
{
	std::string Utilities::loadFile(std::string filename)
	{
		std::ifstream file(filename.c_str());

		//make sure we opened the file correctly
		if (!file)
		{
			std::cerr << "Could not open file: " + filename << std::endl;
			return "";
		}

		std::string line = "";
		std::string result = "";

		while(getline(file, line))
		{
			result = result + "\n" + line;
		}

		file.close();
		return result;
	}
}
