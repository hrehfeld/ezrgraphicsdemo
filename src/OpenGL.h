/*******************************************************************************
 *  OpenGL.h
 *
 *  (C) 2003 AG Computergraphik <cg@uni-koblenz.de>
 *           Universitaet Koblenz-Landau
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/

#ifndef _OPENGL_H_
#define _OPENGL_H_

//--- Include headers ----------------------------------------------------------

#ifdef WIN32
#  include <windows.h>
#endif

#ifdef __APPLE__
#  include <GLUT/gl.h>
#else

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"
#ifdef LINUX
#  include <GL/glext.h>
#elif MINGW
#  include <GL/glext.h>
#endif

#ifdef WIN32
#  include <GL/wglew.h>
#elif LINUX
#  include <GL/glxew.h>
#elif MINGW
#  include <GL/wglew.h>
#endif

#endif

#include <iostream>
#include <string>
#include "EzrException.h"

namespace Ezr
{
	class OpenGl {
	public:
		static void checkError(const char* reason) {
			GLenum error = glGetError();
			if (!(error == GL_NO_ERROR)) {
			    std::stringstream errorString;
                errorString << reason << ": "<< gluErrorString(error);
                std::cerr << errorString << std::endl;
                throw GLException(errorString.str());                      
		    }
		}
	};
}



#endif /* _OPENGL_H_ */
