#ifndef _DEFERREDPOINTLIGHTING_H_
#define _DEFERREDPOINTLIGHTING_H_

#include <string>

#include "Shader.h"
#include "gl/GlBindShader.h"

#include <Eigen/Geometry>

using namespace Eigen;


namespace Ezr
{

	class Texture;
	class Camera;
	class PointLight;

	class DeferredPointLighting : public Shader
	{
	public:
		DeferredPointLighting();
	    virtual ~DeferredPointLighting();

		virtual void bind(const PointLight& light, const Matrix4f& modelView);
		virtual void unbind();

		GLuint getProgram() { return _program.getProgram(); }
		
	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;
		/**
		 * How much light still contributes where the radius of the sphere cuts off
		 */
		static const float epsilon;

		GlBindShader _program;
		
	};
}

#endif /* _DEFERREDPOINTLIGHTING_H_ */
