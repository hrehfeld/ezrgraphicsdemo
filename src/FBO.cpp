/*******************************************************************************
 *  FBO.cpp
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/
#include<string>
#include "FBO.h"


namespace EZR{

	//// FBO ///////////////////////////////////////////////////////////////
	//
	// FBO constructor: create our FBO
	////////////////////////////////////////////////////////////////////////
	FBO::FBO(int textureResX, int textureResY, bool renderbuffer)
		:_fboID(0), _rboID(0), _useDepth(renderbuffer),_textureResX(textureResX),
		_textureResY(textureResY)
	{
		generateFBO();
	}

	//// ~FBO //////////////////////////////////////////////////////////////
	//
	// FBO destructor: call release
	////////////////////////////////////////////////////////////////////////
	FBO::~FBO()
	{
		release();
	}

	
	//// release ////////////////////////////////////////////////////////////
	//
	// free our memory and delete the FBO and 
	// RBO (RenderBufferObject)
	////////////////////////////////////////////////////////////////////////
	void FBO::release()
	{
		glDeleteFramebuffersEXT(1, &_fboID);
		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not delete framebuffer in FBO::release()"); 
		
		if(_useDepth)
		{
			glDeleteRenderbuffersEXT(1, &_rboID);		
			if(glGetError() != GL_NO_ERROR)
				throw std::string("Error: Could not delete renderbuffer in FBO::release()"); 
		}
	}

	//// CREATE RBO ////////////////////////////////////////////////////////
	//
	// This creates our renderbuffer. We need this for later 
	// attaching a depth buffer
	////////////////////////////////////////////////////////////////////////
	void FBO::generateRBO()
	{
		glGenRenderbuffersEXT(1, &_rboID);
		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not generate renderbuffer in FBO::generateRBO()"); 
	}

	//// CREATE FBO ////////////////////////////////////////////////////////
	//
	// This creates our FBO and if we need a depth buffer, 
	// create a renderbuffer 
	////////////////////////////////////////////////////////////////////////
	void FBO::generateFBO()
	{
		glGenFramebuffersEXT(1, &_fboID);
		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not generate renderbuffer in FBO::generateFBO()");

		if(_useDepth)
		{
			generateRBO();
		}
	}

	//// BIND //////////////////////////////////////////////////////////////
	//
	// This function binds the FBO as current renderbuffer
	////////////////////////////////////////////////////////////////////////
	void FBO::bind(){
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboID);
		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not bind framebuffer"); 

		if(_useDepth)
		{
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rboID);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _textureResX, _textureResY);
			
			if(glGetError() != GL_NO_ERROR)
				throw std::string("Error: Could not bind renderbuffer"); 
		}
	}

	//// ATTACH FBO TEXTURE ////////////////////////////////////////////////
	//
	// Attach a new texture to the FBO
	////////////////////////////////////////////////////////////////////////
	void FBO::attachFBOTexture(GLenum attachment, GLuint target, GLuint texID)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, target, texID, 0);

		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not attach texture to framebuffer"); 
	}

	//// ATTACH RBO ////////////////////////////////////////////////////////
	//
	// Attach a renderbuffer, e.g. a depth buffer
	////////////////////////////////////////////////////////////////////////
	void FBO::attachRBO(GLenum attachment)
	{
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _rboID);
	}

	//// GET MAX COLOR ATTACHMENTS /////////////////////////////////////////
	//
	// Returns max supported color attachment points of your GPU
	////////////////////////////////////////////////////////////////////////
	int FBO::getMaxColorAttach()const
	{
		GLint max = 0;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &max);
		return (int)max;
	}

	//// DISABLE FBO ///////////////////////////////////////////////////////
	//
	// Switch back to screen buffer, screen bufferID=0
	////////////////////////////////////////////////////////////////////////

	void FBO::unbindFBO() 
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not bind default fixed function framebuffer"); 
		if(_useDepth){
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
			if(glGetError() != GL_NO_ERROR)
				throw std::string("Error: Could not unbind renderbuffer"); 
		}
	}

	//// CHECK FBO ////////////////////////////////////////////////////////
	//
	// This function checks the fbo status 
	////////////////////////////////////////////////////////////////////////
	void FBO::checkFBO()
	{
		GLenum error = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		
		//Console output
		printf("\n");
		printf("FBO debugging: \n");
		printf("-----------------------------------------------------------------\n");
		printf("Max supported FBO color attachments (textures) of your GPU: %d\n",getMaxColorAttach());

		if(error==GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT){
			printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT){
			printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT){
			printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT){
			printf("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT){
			printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT){
			printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_UNSUPPORTED_EXT){
			printf("GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_COMPLETE_EXT){
			printf("FBO complete!\n");
		}
	}
}
