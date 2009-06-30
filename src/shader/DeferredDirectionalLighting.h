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

	class DeferredDirectionalLighting : public Shader
	{
	public:
		DeferredDirectionalLighting(const Vector3f* lightDirection,
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
	    virtual ~DeferredDirectionalLighting();

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

		GlBindShader _program;
		const Vector3f* _lightDirection;
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

#endif /* _DEFERREDDIRECTIONALLIGHTING_H_ */
