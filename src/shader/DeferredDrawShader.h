#ifndef _DEFERREDDRAWSHADER_H_
#define _DEFERREDDRAWSHADER_H_

#include <string>

#include "Shader.h"
#include "gl/GlBindShader.h"

namespace Ezr
{

	class Texture;

	class DeferredDrawShader : public Shader
	{
	public:
	    DeferredDrawShader(const Texture* colorMap, const Texture* normalMap);
	    virtual ~DeferredDrawShader();

		virtual void bind();
		virtual void unbind();
		
	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;

		GlBindShader _program;
		const Texture* _colorMap;
		const Texture* _normalMap;
	};
}

#endif /* _DEFERREDDRAWSHADER_H_ */
