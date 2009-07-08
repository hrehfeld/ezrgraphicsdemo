#ifndef FONT_H 
#define FONT_H 

#include <string>
#include <utility>
#include "glfont2.h"

namespace Ezr
{
    class Viewport;

    class Font{ 

        public: 
            Font(std::string name, Viewport* vp); 
            ~Font(); 
            
            void renderText(std::string text);
            
        private:
            bool createFont(std::string path);

            GLuint _texID;
            Viewport* _vp;
            glfont::GLFont _font;
    };
}

#endif //FONT_H 

