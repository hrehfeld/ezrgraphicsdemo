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
#include "FboColorAttachment.h"
#include <sstream>

using namespace std;

namespace Ezr
{
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
        _attachmentId = 0x8CE0;

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
	}
	
	//// release ////////////////////////////////////////////////////////////
	//
	// free our memory and delete the Fbo and 
	// RBO (RenderBufferObject)
	////////////////////////////////////////////////////////////////////////
	void Fbo::release()
	{
		glDeleteFramebuffersEXT(1, &_fboID);
		OpenGl::checkError("Couldn't delete FBO");
		
		if(_useDepth)
		{
			glDeleteRenderbuffersEXT(1, &_rboID);
			OpenGl::checkError("Couldn't delete FBO renderbuffer");
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
		OpenGl::checkError("Couldn't generate Fbo");
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
		
		OpenGl::checkError("Couldn't generate fbo renderbuffer");

		if((renderTargets & Depth) != None)
		{
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
                                         GL_DEPTH_ATTACHMENT_EXT,
                                         GL_RENDERBUFFER_EXT,
										 _rboID);
            OpenGl::checkError("Couldn't attach renderbuffer");
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
		OpenGl::checkError("Couldn't bind FBO");
		
		if(_useRbo)
		{
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rboID);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _textureResX, _textureResY);
			
			OpenGl::checkError("Couldn't bind FBO renderbuffer");
		}

        setDrawBuffers();
	}

	void Fbo::setDrawBuffers()
	{
		int attachments = _colorAttachments.size();
        std::vector<GLenum> targets;
		
        map<std::string, ColorAttachmentPtr>::iterator it;
		for (it = _colorAttachments.begin(); it != _colorAttachments.end(); ++it) 
        {
            targets.push_back((*it).second->getAttachmentId());
		}
        
        if(!targets.empty())
		    glDrawBuffers(attachments, &targets[0]);
		
        OpenGl::checkError("Couldn't set FBO draw buffers");
	}

	void Fbo::addColorAttachment(Texture* buffer, const std::string& name)
	{
		bind();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
								  _attachmentId,
								  GL_TEXTURE_2D,
								  buffer->getId(),
								  0);
		OpenGl::checkError("Couldn't attach colorbuffer to FBO");

        _colorAttachments.insert(make_pair(name, new FboColorAttachment(buffer, _attachmentId)));
        _attachmentId +=1;
	}

	Texture* Fbo::getColorAttachment(const std::string& name)
	{
        if(_colorAttachments.find(name) == _colorAttachments.end())
            throw GLException("couldn't get colorAttachment: " + name);
        return _colorAttachments[name]->getTexture();
	}


	void Fbo::setDrawBuffer(const std::string& name)
	{
        glDrawBuffer(_colorAttachments[name]->getAttachmentId());
	}

	
	void Fbo::clearColorAttachment(std::string& name, float r, float g, float b, float a)
	{
		bind();
		setDrawBuffer(name);
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);

		std::stringstream msg;
		msg << "after FBO colorbuffer " << name << " clear";
		OpenGl::checkError(msg.str().c_str());
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
			OpenGl::checkError("Couldn't unbind FBO renderbuffer");
		}

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		OpenGl::checkError("Couldn't unbind FBO");
		
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
            throw GLException("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT){
            throw GLException("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT){
            throw GLException("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT){
            throw GLException("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT){
            throw GLException("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT){
            throw GLException("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_UNSUPPORTED_EXT){
            throw GLException("GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");
		}
		if(error==GL_FRAMEBUFFER_COMPLETE_EXT){
			printf("Fbo complete!\n");
		}
		OpenGl::checkError("FBO error checking");
		
	}
}
