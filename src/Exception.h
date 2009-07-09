#ifndef EXCEPTION_H 
#define EXCEPTION_H 

#include <exception>
#include <string>
#include <sstream>
#include "OpenGL.h"

namespace Ezr
{
    class Exception : public std::exception { 

        public: 
            virtual const char* what() const = 0;
    };

    class GlException : public Exception {

        public:
            GlException();
            GlException(std::string err);

            virtual const char* what() const
            {
                GLenum error = glGetError();
                std::stringstream errorString;
                errorString << _error << gluErrorString(error);
                return errorString.str().c_str();
            }

        private:
            std::string _error;
    };

    GlException::GlException():_error("OpenGL Exception occured!")
    {
    }

    GlException::GlException(std::string err) : _error(err)
    {
    }
}
#endif //EXCEPTION_H 

