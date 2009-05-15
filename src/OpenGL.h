/*******************************************************************************
 *  OpenGL.h
 *
 *  (C) 2003 AG Computergraphik <cg@uni-koblenz.de>
 *           Universitaet Koblenz-Landau
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/

#ifndef _ER_OPENGL_H
#define _ER_OPENGL_H

//--- Include headers ----------------------------------------------------------

#ifdef WIN32
#  include <windows.h>
#endif

#ifdef __APPLE__
#  include <GLUT/gl.h>
#else
//#  include <stdlib.h> 
//#  include <GL/glew.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include "GL/glut.h"
#endif

#endif
