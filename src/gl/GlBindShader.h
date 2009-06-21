#ifndef _GLBINDSHADER_H_
#define _GLBINDSHADER_H_

#include "OpenGL.h"

namespace Ezr
{
	class GlBindShader
	{
	public:
		GlBindShader(const std::string& vertexshader, const std::string& fragmentshader);
		virtual ~GlBindShader();

		/**
		 * Bind this shader
		 */
		void bind();
		/**
		 * unbind shader
		 */
		void unbind();
	private:
		GLhandleARB _program;
		
		GLhandleARB compileShader(const std::string& shader, bool vertex);
		std::string getShaderInfoLog(GLuint program);
		std::string getProgramInfoLog(GLuint program);
		
	};
}

#endif /* _GLBINDSHADER_H_ */
