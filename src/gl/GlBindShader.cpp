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

		if (!success(_program, false))
		{
			std::cout << getInfoLog(_program, false) << std::endl;
		}
	}

	GLhandleARB GlBindShader::compileShader(const std::string& shader, const bool vertex) {
		GLint typeBit = (!vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	
		GLhandleARB handle =  glCreateShader(typeBit);


		//&(shader.c_str()) doesn't work
		const char* fuckCplusplus = shader.c_str();
		glShaderSource(handle, 1, &fuckCplusplus, NULL);
		glCompileShader(handle);

		if (!success(handle, true))
		{
			std::cout << getInfoLog(handle, true) << std::endl;
		}

		return handle;
	}

	bool GlBindShader::success(GLuint handle, bool shader) {
		int success = 0;
		(shader)
			? glGetShaderiv(handle, GL_COMPILE_STATUS, &success)
			: glGetProgramiv(handle, GL_LINK_STATUS, &success);

		return (success != GL_FALSE);
	}
	
	std::string GlBindShader::getInfoLog(GLuint handle, bool shader)
	{
	    int infologLength = 0;
		(shader)
			? glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infologLength)
			: glGetProgramiv(handle, GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength <= 0)
	    {
			return "";
		}

		char *infoLog = (char *)malloc(infologLength);
		
		int charsWritten  = 0;
		(shader) 
			? glGetShaderInfoLog(handle, infologLength, &charsWritten, infoLog)
			: glGetProgramInfoLog(handle, infologLength, &charsWritten, infoLog);
		
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
