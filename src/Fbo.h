#ifndef _Fbo_H
#define _Fbo_H

#include "OpenGL.h"
#include <vector>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

namespace Ezr{

	class Texture;
    class FboColorAttachment;

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
        void generateFbo();

		//This enables our Fbo as current buffer to render in
		void bind();

		/**
		 * attach a new texture to our fbo, target
		 */
		void addColorAttachment(Texture* buffer, const std::string& name);

		Texture* getColorAttachment(const std::string& name);
        		
		/**
		 * Clear a certain color attachment
		 *
		 * Careful: this messes with gldrawbuffers, so you need to
		 * rebind other fbos (that you currently want to use) after
		 * using this.
		 */
		void clearColorAttachment(std::string& name, float r, float g, float b, float a);
		
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

		/**
		 * Set the Drawbuffers as needed for the fbo
		 */
		void setDrawBuffers();

		void setDrawBuffer(const std::string& name);

		int getWidth() { return _textureResX; }
		int getHeight() { return _textureResY; }
		
	private:
        typedef boost::shared_ptr<FboColorAttachment> ColorAttachmentPtr;

        /**
		 * creates a RenderBufferObject, e.g. a depthbuffer
		 */
		void generateRbo(short type);

		GLuint _fboID;
		GLuint _rboID;
		int _textureResX;
		int _textureResY;
		bool _useDepth, _useStencil, _useRbo;
        int _attachmentId;

		std::map<std::string, int> _colorBufferNames;
		std::vector<Texture*> _colorBuffers;
		static std::vector<unsigned int> _glColorBufferEnums;
      
        std::map<std::string, ColorAttachmentPtr> _colorAttachments;
	};
}
#endif
