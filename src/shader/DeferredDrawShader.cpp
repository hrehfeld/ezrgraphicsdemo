#include "DeferredDrawShader.h"

#include "Texture.h"
#include "Shader.h"
#include "Utilities.h"

using namespace std;

namespace Ezr
{
	const string DeferredDrawShader::vertexShaderPath("res/shaders/deferred/basic.vert");
	const string DeferredDrawShader::fragmentShaderPath("res/shaders/deferred/basic.frag");
																								  
	DeferredDrawShader::DeferredDrawShader()
		: _program(Utilities::loadFile(DeferredDrawShader::vertexShaderPath),
				   Utilities::loadFile(DeferredDrawShader::fragmentShaderPath)),
	{
	}

	void DeferredDrawShader::bind()
	{
		_program.bind();
		
	}

	void DeferredDrawShader::unbind()
	{
		_program.unbind();
	}

	DeferredDrawShader::~DeferredDrawShader()
	{
	}
}
