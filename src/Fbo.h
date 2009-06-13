/*******************************************************************************
 *  Fbo.h
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/

#ifndef _Fbo_H
#define _Fbo_H

#include "OpenGL.h" 

namespace Ezr{
	
	class Fbo {

	public:

		Fbo(int textureResX, int textureResY, bool renderbuffer=false);
		~Fbo();

		//This creates a RenderBufferObject, e.g. a depthbuffer
		void generateRBO();
		//This creates our FrameBufferObject
		void generateFbo();

		//This enables our Fbo as current buffer to render in
		void bind();

		//This attaches a new texture to our fbo, target
		//attachement: e.g. GL_COLOR_ATTACHMENT0_EXT, 
		//target:      e.g. GL_TEXTURE_2D, 
		//texID:       the texture ID 
		void attachFboTexture(GLenum attachment, GLuint target, GLuint texID);
		
		//This attaches our RenderBufferObject
		void attachRBO(GLenum attachment);

		GLuint getFboID() const {return _fboID;}
		GLuint getRBOID() const {return _rboID;}

		//returns max supported color attachement points of your GPU.
		//e.g. for 3 color attachement points you can attach 3 textures to the fbo
		int getMaxColorAttach()const;

		//switch back to ScreenBuffer 0
		void unbindFbo();
		void checkFbo();
		void release();

	private:
		GLuint _fboID;
		GLuint _rboID;
		int _textureResX;
		int _textureResY;
		bool _useDepth;
	};
}
#endif