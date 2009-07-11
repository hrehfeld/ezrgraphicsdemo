#include "Font.h"
#include "Viewport.h"
#include "OpenGL.h"
#include <iostream>

using namespace glfont;

namespace Ezr{
    
    Font::Font(std::string path, Viewport* vp): _vp(vp)
    {   
        if(!createFont(path))
            std::cerr << "could not create font" << std::endl;
    }

    Font::~Font()
    {
        _font.Destroy();
    }

    bool Font::createFont(std::string path)
    {
        glGenTextures(1, &_texID);
        	
         //Create font
        if (!_font.Create(path, _texID))
            return false;
        return true;
    }

    void Font::renderText(std::string text)
    {   
        glClearColor(0.0, 0.0, 0.0, 1.0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
        gluOrtho2D(0.0, 512, 0.0, 512);
 
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
 
		glColor3f(1.0f, 1.0f, 1.0f);
        
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       
        _font.Begin();
        _font.DrawString(text, 0.05f, 450, 500);      

        glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
    }
}           
