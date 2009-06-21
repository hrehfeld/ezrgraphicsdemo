#ifndef _GLBINDSHADER_H_
#define _GLBINDSHADER_H_

#include "OpenGl.h"

namespace Ezr
{
	class GlBindShader
	{
	public:
		GlBindShader(const std::string& vertexshader, const std::string& fragmentshader);
		virtual ~GlBindShader();

	private:
		GLhandleARB _program;
		
		GLhandleARB compileShader(const std::string& shader, bool vertex);
	};
}

#endif /* _GLBINDSHADER_H_ */
