#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>


namespace Ezr
{
	class Image;
	
	class Texture
	{
	public:
	    Texture(const Image& img);
	    virtual ~Texture();

		void setAnisotropicFiltering(int anisotropic);
		void bind();
	private:
		unsigned int _id;
	};
}

#endif /* _TEXTURE_H_ */
