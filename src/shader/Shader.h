#ifndef _SHADER_H_
#define _SHADER_H_

namespace Ezr
{
	class Shader
	{
	public:

		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}

#endif /* _SHADER_H_ */
