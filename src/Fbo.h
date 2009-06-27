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
#include <vector>

namespace Ezr{

	class Texture;

	class Fbo {

	public:

        enum Attachment
        {
            None = 0,
            Depth = 0x1,
            Stencil = Depth << 1,
        };

		Fbo(int textureResX, int textureResY, short type);
		~Fbo();
		
		//This creates our FrameBufferObject
        void generateFbo(unsigned short att = None);

		//This enables our Fbo as current buffer to render in
		void bind();

		/**
		 * attach a new texture to our fbo, target
		 */
		void attachColorbuffer();

		const Texture* getColorAttachmentId(unsigned int colorAttachment);
		
		GLuint getFboID() const {return _fboID;}
		GLuint getRBOID() const {return _rboID;}

		//returns max supported color attachement points of your GPU.
		//e.g. for 3 color attachement points you can attach 3 textures to the fbo
		int getMaxColorAttach()const;

		//switch back to ScreenBuffer 0
		void unbindFbo();
		void checkFbo();
		void release();

		static bool staticInit();

	private:
        /**
		 * creates a RenderBufferObject, e.g. a depthbuffer
		 */
		void generateRbo(short type);

		GLuint _fboID;
		GLuint _rboID;
		int _textureResX;
		int _textureResY;
		bool _useDepth, _useStencil;

		std::vector<Texture*> _colorBuffers;
		static std::vector<unsigned int> _glColorBufferEnums;
	};
}
#endif
