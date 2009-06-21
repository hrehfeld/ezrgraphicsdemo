#include "boost/filesystem.hpp"
#include <iostream>
#include "GlBindShader.h"
#include "OpenGl.h"

namespace Ezr
{
	GlBindShader::GlBindShader(const std::string& vertexshader, const std::string& fragmentshader) {
		std::cout << "trying to create program... ";
		
		_program = glCreateProgram();

		std::cout << "created program... ";

		GLhandleARB vertex, fragment;
		if (!vertexshader.empty())
		{
			vertex = compileShader(vertexshader, true);
			glAttachShader(_program, vertex);
			std::cout << "attached vertexshader... ";
		}
		else {
			std::cerr << "Vertexshader must be set!" << std::endl;
			return;
		}

		if (!fragmentshader.empty())
		{
			fragment = compileShader(fragmentshader, false);
			glAttachShader(_program, fragment);
			std::cout << "attached fragmentshader... ";
		}
		else {
			std::cerr << "fragmentshader must be set!" << std::endl;
			return;
		}

		glLinkProgram(_program);
		std::cout << "linked program... ";
	
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
