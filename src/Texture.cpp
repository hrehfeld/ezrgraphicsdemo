#include "Texture.h"

#include "OpenGl.h"
#include "Image.h"

namespace Ezr
{
	Texture::Texture(const Image& img)
	{

		glGenTextures(1, &_id);
		bind();

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexImage2D(GL_TEXTURE_2D, 0,
					 GL_RGB,
					 img.getWidth(), img.getHeight(),
					 0,
					 img.getFormat(), img.getType(),
					 img.getData());
		
		//with FBO_EXT support, or glGenerateMipmap(GL_TEXTURE_2D)
		//make sure all drivers gen mipmaps
		glEnable(GL_TEXTURE_2D);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);
		
	}

	void Texture::setAnisotropicFiltering(int anisotropic)
	{
		bind();
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
	}

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_id);
	}
}
