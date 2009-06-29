#include "DeferredDrawShader.h"

#include "Texture.h"
#include "Shader.h"
#include "Utilities.h"

using namespace std;

namespace Ezr
{
	const string DeferredDrawShader::vertexShaderPath("res/shaders/deferred/basic.vert");
	const string DeferredDrawShader::fragmentShaderPath("res/shaders/deferred/basic.frag");
																								  
	DeferredDrawShader::DeferredDrawShader(const Texture* colorMap, const Texture* normalMap)
		: _program(Utilities::loadFile(DeferredDrawShader::vertexShaderPath),
				   Utilities::loadFile(DeferredDrawShader::fragmentShaderPath)),
		  _colorMap(colorMap),
		  _normalMap(normalMap)
	{
	}

	void DeferredDrawShader::bind()
	{
		glActiveTexture(GL_TEXTURE0);
		_colorMap->bind();
		glEnable(GL_TEXTURE_2D);

		// //multitexturing
		glActiveTexture(GL_TEXTURE1);
		_normalMap->bind();
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		_program.bind();
		
		//tell the shader about uniforms
		GLint program = _program.getProgram();
		glUniform1i(glGetUniformLocation(program, "colorMap"), 0);
		glUniform1i(glGetUniformLocationARB(program, "normalMap"), 1);
	}

	void DeferredDrawShader::unbind()
	{
		_program.unbind();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		
	}

	DeferredDrawShader::~DeferredDrawShader()
	{
		delete _colorMap;
		delete _normalMap;
	}
}
