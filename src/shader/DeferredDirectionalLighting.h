#ifndef _DEFERREDDIRECTIONALLIGHTING_H_
#define _DEFERREDDIRECTIONALLIGHTING_H_

#include <string>

#include "Shader.h"
#include "gl/GlBindShader.h"

#include <Eigen/Geometry>

using namespace Eigen;


namespace Ezr
{

	class Texture;
	class Camera;
	class DirectionalLight;

	class DeferredDirectionalLighting : public Shader
	{
	public:
		DeferredDirectionalLighting();
	    virtual ~DeferredDirectionalLighting();

		virtual void bind(const DirectionalLight& light, const Matrix4f& modelView);
		virtual void unbind();

		GLuint getProgram() { return _program.getProgram(); }
		
	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;

		GlBindShader _program;
	};
}

#endif /* _DEFERREDDIRECTIONALLIGHTING_H_ */
