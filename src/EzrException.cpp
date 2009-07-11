#include "EzrException.h"


namespace Ezr
{
	EzrException::~EzrException() throw () {}

	GLException::~GLException() throw () {} 

	GLException::GLException() : _error("GLException")
	{
	}

	GLException::GLException(std::string err) : _error(err)
	{
	}
	
}

