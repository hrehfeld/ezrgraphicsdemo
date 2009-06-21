#include "boost/filesystem.hpp"
#include <iostream>
#include "GlBindShader.h"
#include "OpenGl.h"

Ezr::GlBindShader::GlBindShader(const std::string& vertexshader, const std::string& fragmentshader) {
	_program = glCreateProgram();

	GLhandleARB vertex, fragment;
	if (!vertexshader.empty())
	{
		vertex = compileShader(vertexshader, true);
		glAttachShader(_program, vertex);
	}
	if (!fragmentshader.empty())
	{
		fragment = compileShader(fragmentshader, false);
		glAttachShader(_program, fragment);
	}

	glLinkProgram(_program);
	
}

GLhandleARB Ezr::GlBindShader::compileShader(const std::string& shader, const bool vertex) {
	GLint typeBit = (!vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	
	GLhandleARB handle =  glCreateShader(typeBit);


	//&(shader.c_str()) doesn't work
	const char* fuckCplusplus = shader.c_str();
	glShaderSource(handle, 1, &fuckCplusplus, NULL);
	glCompileShader(handle);

	return handle;
}

Ezr::GlBindShader::~GlBindShader() {}

