#include "Texture.h"

#include <iostream>

#include "OpenGl.h"
#include "Image.h"

namespace Ezr
{
	
	Texture::Texture(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type)
	{
		init(width, height, internalFormat, format, type, NULL);
	}
	
	Texture::Texture(const Image& img)
	{
		init(img.getWidth(), img.getHeight(), GL_RGB, img.getFormat(), img.getType(), img.getData());
	}

	void Texture::init(int width,
					   int height,
					   unsigned int internalFormat,
					   unsigned int format,
					   unsigned int type,
					   unsigned char* data)
	{
		glGenTextures(1, &_id);
		bind();

		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexImage2D(GL_TEXTURE_2D, 0,
					 internalFormat,
					 width, height,
					 0,
					 format, type,
					 data);
		OpenGl::checkError("Couldn't generate Texture");
		
		
		//with FBO_EXT support, or glGenerateMipmap(GL_TEXTURE_2D)
		//make sure all drivers gen mipmaps
		glEnable(GL_TEXTURE_2D);
		// glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);
		
	}

	void Texture::setAnisotropicFiltering(int anisotropic)
	{
		bind();
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_id);
	}
}
