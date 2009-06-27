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


		GLhandleARB getProgram() { return _program; }

	private:
		GLhandleARB _program;
		
		GLhandleARB compileShader(const std::string& shader, bool vertex);
		/**
		 * Get the info log for shader compilation or program linking
		 *
		 * @param program handle to shader or program
		 * @param shader true for shader info, false for program info
		 */
		std::string getInfoLog(GLuint program, bool shader);

		bool success(GLuint handle, bool shader);
	};
}

#endif /* _GLBINDSHADER_H_ */
