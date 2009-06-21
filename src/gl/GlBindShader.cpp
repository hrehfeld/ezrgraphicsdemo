#include "boost/filesystem.hpp"
#include <iostream>
#include "GlBindShader.h"
#include "OpenGl.h"

namespace Ezr
{
	GlBindShader::GlBindShader(const std::string& vertexshader, const std::string& fragmentshader) {

		GLhandleARB vertex, fragment;
		if (!vertexshader.empty())
		{
			vertex = compileShader(vertexshader, true);
		}
		else {
			std::cerr << "Vertexshader must be set!" << std::endl;
			return;
		}

		if (!fragmentshader.empty())
		{
			fragment = compileShader(fragmentshader, false);
		}
		else {
			std::cerr << "fragmentshader must be set!" << std::endl;
			return;
		}

		_program = glCreateProgram();
		glAttachShader(_program, vertex);
		glAttachShader(_program, fragment);
		glLinkProgram(_program);
	}

	GLhandleARB GlBindShader::compileShader(const std::string& shader, const bool vertex) {
		GLint typeBit = (!vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	
		GLhandleARB handle =  glCreateShader(typeBit);


		//&(shader.c_str()) doesn't work
		const char* fuckCplusplus = shader.c_str();
		glShaderSource(handle, 1, &fuckCplusplus, NULL);
		glCompileShader(handle);

		return handle;
	}

	GlBindShader::~GlBindShader() {}

	void GlBindShader::bind()
	{
		glUseProgram(_program);
	}
}
