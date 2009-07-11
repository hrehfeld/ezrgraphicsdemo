#ifndef EXCEPTION_H 
#define EXCEPTION_H 

#include <exception>
#include <string>
#include <sstream>

namespace Ezr
{
    class Exception : public std::exception { 

        public: 
            virtual const char* what() const = 0;
    };

    class GLException : public Exception {

        public:
            GLException();
            GLException(std::string err);

            virtual const char* what() const
            {                
                return _error.c_str();
            }

        private:
            std::string _error;
    };
}
#endif //EXCEPTION_H 

