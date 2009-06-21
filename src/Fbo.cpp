/*******************************************************************************
 *  Fbo.cpp
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/
#include<string>
#include<iostream>
#include "Fbo.h"
#include <stdio.h>


namespace Ezr{

	//// Fbo ///////////////////////////////////////////////////////////////
	//
	// Fbo constructor: create our Fbo
	////////////////////////////////////////////////////////////////////////
	Fbo::Fbo(int textureResX, int textureResY)
		:_fboID(0), _rboID(0), _useDepth(false),_textureResX(textureResX),
		_textureResY(textureResY), _useNormal(false)
	{
		generateFbo();
	}

	//// ~Fbo //////////////////////////////////////////////////////////////
	//
	// Fbo destructor: call release
	////////////////////////////////////////////////////////////////////////
	Fbo::~Fbo()
	{
		try{
			release();
		}catch(std::string e){
			std::cerr << e << std::endl;
		}
	}
	
	//// release ////////////////////////////////////////////////////////////
	//
	// free our memory and delete the Fbo and 
	// RBO (RenderBufferObject)
	////////////////////////////////////////////////////////////////////////
	void Fbo::release()
	{
		glDeleteFramebuffersEXT(1, &_fboID);
		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not delete framebuffer in Fbo::release()"); 
		
		if(_useDepth)
		{
			glDeleteRenderbuffersEXT(1, &_rboID);		
			if(glGetError() != GL_NO_ERROR)
				throw std::string("Error: Could not delete renderbuffer in Fbo::release()"); 
		}
	}	

	//// CREATE Fbo ////////////////////////////////////////////////////////
	//
	// This creates our Fbo and if we need a depth buffer, 
	// create a renderbuffer 
	////////////////////////////////////////////////////////////////////////
	void Fbo::generateFbo(unsigned short att)
	{
		glGenFramebuffersEXT(1, &_fboID);
		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not generate renderbuffer in Fbo::generateFbo()");

		if((att & 0x2) == 0x2)
		{
			generateRbo(att);
		}
	}

    //// CREATE RBO ////////////////////////////////////////////////////////
	//
	// This creates our renderbuffer. We need this for later 
	// attaching a depth buffer
	////////////////////////////////////////////////////////////////////////
	void Fbo::generateRbo(unsigned short att)
    {  
        glGenRenderbuffersEXT(1, &_rboID);
	    if(glGetError() != GL_NO_ERROR)
	    	throw std::string("Error: Could not generate renderbuffer in Fbo::generateRBO()"); 
        _useDepth = true;
	}

	//// BIND //////////////////////////////////////////////////////////////
	//
	// This function binds the Fbo as current renderbuffer
	////////////////////////////////////////////////////////////////////////
	void Fbo::bind(){
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboID);
		if(glGetError() != GL_NO_ERROR)
		{
			std::cout << "fbo binding errored out... ";
			throw std::string("Error: Could not bind framebuffer");
		}

		if(_useDepth)
		{
			std::cout << "binding depth buffer" << std::endl;
			
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rboID);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _textureResX, _textureResY);
			
			if(glGetError() != GL_NO_ERROR)
				throw std::string("Error: Could not bind renderbuffer"); 
		}
	}

	//// ATTACH Fbo TEXTURE ////////////////////////////////////////////////
	//
	// Attach a new texture to the Fbo
	////////////////////////////////////////////////////////////////////////
	void Fbo::attachFboTexture(GLenum attachment, GLuint target, GLuint texID)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, target, texID, 0);

		if(glGetError() != GL_NO_ERROR)
			throw std::string("Error: Could not attach texture to framebuffer"); 
	}

	//// ATTACH RBO ////////////////////////////////////////////////////////
	//
	// Attach a renderbuffer, e.g. a depth buffer
	////////////////////////////////////////////////////////////////////////
	void Fbo::attachRBO(GLenum attachment)
	{
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _rboID);
	}

	//// GET MAX COLOR ATTACHMENTS /////////////////////////////////////////
	//
	// Returns max supported color attachment points of your GPU
	////////////////////////////////////////////////////////////////////////
	int Fbo::getMaxColorAttach()const
	{
		GLint max = 0;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &max);
		return (int)max;
	}

	//// DISABLE Fbo ///////////////////////////////////////////////////////
	//
	// Switch back to screen buffer, screen bufferID=0
	////////////////////////////////////////////////////////////////////////

	void Fbo::unbindFbo() 
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

	//// CHECK Fbo ////////////////////////////////////////////////////////
	//
	// This function checks the fbo status 
	////////////////////////////////////////////////////////////////////////
	void Fbo::checkFbo()
	{
		GLenum error = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		
		//Console output
		printf("\n");
		printf("Fbo debugging: \n");
		printf("-----------------------------------------------------------------\n");
		printf("Max supported Fbo color attachments (textures) of your GPU: %d\n",getMaxColorAttach());

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
			printf("Fbo complete!\n");
		}
	}
}
