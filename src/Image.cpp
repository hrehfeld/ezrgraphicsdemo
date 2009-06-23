#include "Image.h"

using namespace std;

namespace Ezr
{
	bool Image::ilInitialised = false;
	
	Image::Image(string filename)
	{
		Image::init();

		ilGenImages(1, &_image);

		ilBindImage(_image);
		ilLoadImage(filename.c_str());

		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR) {
			sprintf("%s/n", iluErrorString(Error)); 
		}
	}

	void Image::init()
	{
		if (Image::ilInitialised)
		{
			return;
		}

		ilInit();
	}

	Image::~Image()
	{
		ilDeleteImages(1, &_image);	
	}

	ILinfo Image::getInfo() const 
	{
		ilBindImage(_image);
		ILinfo info;
		iluGetImageInfo(&info);
		return info;
	}
}