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
        _font.Begin();
		float scale = 0.1f;
        glTranslatef(3, 300, 0);
        glScalef(scale, scale, scale);
        _font.DrawString(text, 0.5f, 0.5f);
    }
}           
