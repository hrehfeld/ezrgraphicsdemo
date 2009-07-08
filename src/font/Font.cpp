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
        glScalef(3.0, 3.0, 3.0);
        glTranslatef(30, 30, 0);
        _font.DrawString(text, 0.5f, 0.5f);
    }
}           
