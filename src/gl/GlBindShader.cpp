#include "boost/filesystem.hpp"
#include <iostream>
#include "GlBindShader.h"
#include "OpenGl.h"
#include <string>

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

		int success = 0;
		glGetProgramiv(_program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			std::cout << getProgramInfoLog(_program) << std::endl;
		}
	}

	GLhandleARB GlBindShader::compileShader(const std::string& shader, const bool vertex) {
		GLint typeBit = (!vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	
		GLhandleARB handle =  glCreateShader(typeBit);


		//&(shader.c_str()) doesn't work
		const char* fuckCplusplus = shader.c_str();
		glShaderSource(handle, 1, &fuckCplusplus, NULL);
		glCompileShader(handle);

		int success = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			std::cout << getShaderInfoLog(handle) << std::endl;
		}

		return handle;
	}
	
	std::string GlBindShader::getShaderInfoLog(GLuint shader)
	{
	    int infologLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	    if (infologLength <= 0)
	    {
			return "";
		}

		char *infoLog = (char *)malloc(infologLength);
		
		int charsWritten  = 0;
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		
		std::string result(infoLog, infologLength);
		free(infoLog);
		return result;
	}

	std::string GlBindShader::getProgramInfoLog(GLuint program)
	{

	    int infologLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength <= 0)
	    {
			return "";
	    }

		char *infoLog = (char *)malloc(infologLength);
		
		int charsWritten  = 0;
		glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
		
		std::string result(infoLog, infologLength);
		free(infoLog);
		return result;
	}	


	GlBindShader::~GlBindShader()
	{
		
	}

	void GlBindShader::bind()
	{
		glUseProgram(_program);
	}

	void GlBindShader::unbind()
	{
		glUseProgram(0);
	}
}
