#ifndef EXCEPTION_H 
#define EXCEPTION_H 

#include <exception>
#include <string>
#include <sstream>

namespace Ezr
{
    class EzrException : public std::exception { 

	public: 
		virtual const char* what() const throw () = 0;
		virtual ~EzrException() throw ();
    };

    class GLException : public EzrException {

	public:
		GLException();
		GLException(std::string err);
		virtual ~GLException() throw ();

		virtual const char* what() const throw ()
            {                
                return _error.c_str();
            }

	private:
		std::string _error;
    };
}
#endif //EXCEPTION_H 

