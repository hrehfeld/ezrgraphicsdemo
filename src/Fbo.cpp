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
#include <sstream>

using namespace std;

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
		 _textureResX(textureResX),
		 _textureResY(textureResY),
		 _useDepth(false),
		 _useStencil(false),
         _useRbo(false)
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
		}catch(string e){
			std::cerr << e << std::endl;
		}

		for (vector<Texture*>::iterator it = _colorBuffers.begin();
			 it != _colorBuffers.end();
			 ++it) {
			delete (*it);
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
		OpenGl::printGlError("Couldn't delete FBO");
		
		if(_useDepth)
		{
			glDeleteRenderbuffersEXT(1, &_rboID);
			OpenGl::printGlError("Couldn't delete FBO renderbuffer");
		}
	}	

	//// CREATE Fbo ////////////////////////////////////////////////////////
	//
	// This creates our Fbo and if we need a depth buffer, 
	// create a renderbuffer 
	////////////////////////////////////////////////////////////////////////
	void Fbo::generateFbo()
	{
		glGenFramebuffersEXT(1, &_fboID);
		OpenGl::printGlError("Couldn't generate Fbo");

	}

    //// CREATE RBO ////////////////////////////////////////////////////////
	//
	// This creates our renderbuffer. We need this for later 
	// attaching a depth buffer
	////////////////////////////////////////////////////////////////////////
	void Fbo::generateRbo(short renderTargets)
    {      
        glGenRenderbuffersEXT(1, &_rboID);
        _useRbo = true;	
        bind();        
		
		OpenGl::printGlError("Couldn't generate fbo renderbuffer");

		if((renderTargets & Depth) != None)
		{
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
                                         GL_DEPTH_ATTACHMENT_EXT,
                                         GL_RENDERBUFFER_EXT,
										 _rboID);
            OpenGl::printGlError("Couldn't attach renderbuffer");
		}
		if((renderTargets & Stencil) != None)
		{
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER,
										 GL_STENCIL_ATTACHMENT,
										 GL_RENDERBUFFER,
										 _rboID);
		}
	}

	void Fbo::bind()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboID);
		OpenGl::printGlError("Couldn't bind FBO");
		
		if(_useRbo)
		{
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rboID);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _textureResX, _textureResY);
			
			OpenGl::printGlError("Couldn't bind FBO renderbuffer");
		}

        setDrawBuffers();
	}

	void Fbo::setDrawBuffers()
	{
		int attachments = _colorBuffers.size();
		GLenum* targets = new GLenum[attachments];
		for (int i = 0; i < attachments; ++i) {
			targets[i] = _glColorBufferEnums[i];
		}
		glDrawBuffers(attachments, targets);
		OpenGl::printGlError("Couldn't set FBO draw buffers");

        delete[] targets;
	}

	void Fbo::attachColorbuffer(const std::string& name, GLenum format)
	{
		bind();
		Texture* buffer = new Texture(_textureResX, _textureResY, format, GL_RGBA, GL_FLOAT);
		
		int attachment = _colorBuffers.size();

		// std::cout << "#colorbuffers: "
		// 		  << Fbo::_glColorBufferEnums[attachment] << ", "
		// 		  << GL_COLOR_ATTACHMENT0_EXT << ", "
		// 		  << (_glColorBufferEnums[attachment] == GL_COLOR_ATTACHMENT0_EXT)
		// 		  << ", bufferid: " << buffer->getId()
		// 		  << std::endl;

		// std::cout << "#colorbuffers: " << attachment << std::endl;
		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
								  _glColorBufferEnums[attachment],
								  GL_TEXTURE_2D,
								  buffer->getId(),
								  0);
		OpenGl::printGlError("Couldn't attach colorbuffer to FBO");

		_colorBufferNames.insert(std::make_pair(name, attachment));
		_colorBuffers.push_back(buffer);
	}

	const Texture* Fbo::getColorAttachment(std::string& name)
	{
		return _colorBuffers[_colorBufferNames[name]];
	}

	void Fbo::clearColorAttachment(std::string& name, float r, float g, float b, float a)
	{
		bind();
		int num = _colorBufferNames[name];
		glDrawBuffer(_glColorBufferEnums[num]);
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);

		std::stringstream msg;
		msg << "after FBO colorbuffer " << num << " clear";
		OpenGl::printGlError(msg.str().c_str());
		setDrawBuffers();
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
		if (_useRbo)
		{
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
			OpenGl::printGlError("Couldn't unbind FBO renderbuffer");
		}

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		OpenGl::printGlError("Couldn't unbind FBO");
		
		glDrawBuffer(GL_FRONT);
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
		OpenGl::printGlError("FBO error checking");
		
	}
}
