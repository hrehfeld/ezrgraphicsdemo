#ifndef _DEFERREDDRAWSHADER_H_
#define _DEFERREDDRAWSHADER_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include "Shader.h"
#include "gl/GlBindShader.h"

namespace Ezr
{

	class Texture;

	class DeferredDrawShader : public Shader
	{
	public:
	    DeferredDrawShader( 
            boost::shared_ptr<Texture> colorMap, 
            boost::shared_ptr<Texture> normalMap
            );
	    virtual ~DeferredDrawShader();

		virtual void bind();
		virtual void unbind();
		
	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;

		GlBindShader _program;
		boost::shared_ptr<Texture> _colorMap;
		boost::shared_ptr<Texture> _normalMap;
	};
}

#endif /* _DEFERREDDRAWSHADER_H_ */
