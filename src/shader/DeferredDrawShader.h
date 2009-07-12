#ifndef _DEFERREDDRAWSHADER_H_
#define _DEFERREDDRAWSHADER_H_

#include <string>

#include "Shader.h"
#include "gl/GlBindShader.h"

namespace Ezr
{

	class DeferredDrawShader : public Shader
	{
	public:
	    DeferredDrawShader();
	    virtual ~DeferredDrawShader();

		virtual void bind();
		virtual void unbind();

		GLuint getProgram() { return _program.getProgram(); }

	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;

		GlBindShader _program;
	};
}

#endif /* _DEFERREDDRAWSHADER_H_ */
