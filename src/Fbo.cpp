/*******************************************************************************
 *  Fbo.cpp
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/
#include <string>
#include <iostream>
#include "Fbo.h"
#include "Texture.h"
#include <stdio.h>

namespace Ezr
{

	std::vector<unsigned int> Fbo::_glColorBufferEnums;


	bool Fbo::staticInit()
	{
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT0_EXT);
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT1_EXT);
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT2_EXT);
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT3_EXT);
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT4_EXT);
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT5_EXT);
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT6_EXT);
		_glColorBufferEnums.push_back(GL_COLOR_ATTACHMENT7_EXT);
		return true;
	}
	
	bool bla = Fbo::staticInit();

	//// Fbo ///////////////////////////////////////////////////////////////
	//
	// Fbo constructor: create our Fbo
	////////////////////////////////////////////////////////////////////////
	Fbo::Fbo(int textureResX, int textureResY, short renderTargets)
		:_fboID(0),
		 _rboID(0),
		 _useDepth(false),
		 _useStencil(false),
		 _textureResX(textureResX),
		 _textureResY(textureResY)
	{
		generateFbo();
		if (renderTargets != None)
		{
			generateRbo(renderTargets);
		}
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

		for (std::vector<Texture*>::iterator it = _colorBuffers.begin(); it != _colorBuffers.end(); ++it) {
			delete *it;
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

	}

    //// CREATE RBO ////////////////////////////////////////////////////////
	//
	// This creates our renderbuffer. We need this for later 
	// attaching a depth buffer
	////////////////////////////////////////////////////////////////////////
	void Fbo::generateRbo(short renderTargets)
    {
		bind();
        glGenRenderbuffers(1, &_rboID);
		
	    if(glGetError() != GL_NO_ERROR)
		{
	    	throw std::string("Error: Could not generate renderbuffer in Fbo::generateRBO()");
		}

		if((renderTargets & Depth) != None)
		{
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER,
										 GL_DEPTH_ATTACHMENT,
										 GL_RENDERBUFFER,
										 _rboID);
			_useDepth = true;
		}
		if((renderTargets & Stencil) != None)
		{
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER,
										 GL_STENCIL_ATTACHMENT,
										 GL_RENDERBUFFER,
										 _rboID);
			_useStencil = true;
		}
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

		if(_useDepth || _useStencil)
		{
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rboID);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _textureResX, _textureResY);
			
			if(glGetError() != GL_NO_ERROR)
				throw std::string("Error: Could not bind renderbuffer"); 
		}
	}

	void Fbo::attachColorbuffer()
	{
		bind();
		Texture* buffer = new Texture(_textureResX, _textureResY, GL_RGBA32F, GL_RGBA, GL_FLOAT);
		
		unsigned int colorAttachments = _colorBuffers.size();

		std::cout << "#colorbuffers: "
				  << Fbo::_glColorBufferEnums[colorAttachments] << ", "
				  << GL_COLOR_ATTACHMENT0_EXT << ", "
				  << (_glColorBufferEnums[colorAttachments] == GL_COLOR_ATTACHMENT0_EXT)
				  << ", bufferid: " << buffer->getId()
				  << std::endl;

		std::cout << "#colorbuffers: " << colorAttachments << std::endl;
		

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
								  _glColorBufferEnums[colorAttachments],
								  GL_TEXTURE_2D,
								  buffer->getId(),
								  0);
		OpenGl::printGlError();
		
		_colorBuffers.push_back(buffer);
	}

	const Texture* Fbo::getColorAttachmentId(unsigned int colorAttachment)
	{
		return _colorBuffers[colorAttachment];
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
