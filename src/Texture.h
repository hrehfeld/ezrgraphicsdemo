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
		Texture(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type);
	    virtual ~Texture();

		void setAnisotropicFiltering(int anisotropic);
		void bind() const;
        void unbind() const;
		
        /**
		 * Get the ogl texture id
		 */
		unsigned int getId() {return _id;}
               
	private:
		void init(int width, int height, unsigned int internalFormat, 
            unsigned int format, 
            unsigned int type, 
            unsigned char* data
            );

		unsigned int _id;       
	};
}

#endif /* _TEXTURE_H_ */
