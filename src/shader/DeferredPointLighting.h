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

	class DeferredPointLighting : public Shader
	{
	public:
		DeferredPointLighting(const Vector3f* lightPosition,
							  const float lightRadius,
							  const float attenuation,
							  const Vector2i* screenSize,
							  const Texture* color3_depth1,
							  const Texture* normal2,
							  const float nearPlaneDistance,
							  const float nearSize,
							  Camera* camera,
							  const Matrix4f* modelView,
							  const Matrix4f* modelViewInverse,
							  const Matrix3f* normal,
							  const Matrix3f* normalInverse,
							  const Matrix4f* projection,
							  const Matrix4f* projectionInverse);
	    virtual ~DeferredPointLighting();

		virtual void bind();
		virtual void unbind();

		void setMatrices(const Matrix4f* modelView,
						 const Matrix4f* modelViewInverse,
						 const Matrix3f* normal,
						 const Matrix3f* normalInverse,
						 const Matrix4f* projection,
						 const Matrix4f* projectionInverse);
		
	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;
		/**
		 * How much light still contributes where the radius of the sphere cuts off
		 */
		static const float epsilon;

		GlBindShader _program;
		const Vector3f* _lightPosition;
		const float _lightRadius;
		const float _attenuation;
		const Vector2i* _screenSize;
		const Texture* _color3_depth1;
		const Texture* _normal2;
		const float _nearPlaneDistance;
		const float _nearPlaneSize;
		Camera* _camera;
		const Matrix4f* _modelView;
		const Matrix4f* _modelViewInverse;
		const Matrix3f* _normal;
		const Matrix3f* _normalInverse;
		const Matrix4f* _projection;
		const Matrix4f* _projectionInverse;
	};
}

#endif /* _DEFERREDPOINTLIGHTING_H_ */
