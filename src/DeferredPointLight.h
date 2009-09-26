#ifndef _DEFERREDPOINTLIGHT_H_
#define _DEFERREDPOINTLIGHT_H_

#include <string>
#include "Shader.h"
#include "gl/GlBindShader.h"
#include <boost/shared_ptr.hpp>
#include <Eigen/Geometry>

namespace Ezr
{

	class Texture;
	class Camera;

	class DeferredPointLight : public Shader
	{
	public:
        typedef boost::shared_ptr<DeferredPointLight> PointLightPtr;
		
	    virtual ~DeferredPointLight();

		virtual void bind();
		virtual void unbind();
        void draw();
        void drawGeometry();

        inline void setColorBuffer(Texture* color){_color = color;}
        inline void setNormalBuffer(Texture* normal){_normal = normal;}

        static PointLightPtr create(const Eigen::Vector3f& lightPosition,
				                      const float lightRadius,
				                      const float attenuation,
				                      const Eigen::Vector2i& screenSize,
				                      const float nearPlaneDistance,
				                      const float nearSize,
							          Camera* camera);
		
	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;

        DeferredPointLight(const Eigen::Vector3f& lightPosition,
							 float lightRadius,
							 float attenuation,
							 const Eigen::Vector2i& screenSize,
							 float nearPlaneDistance,
							 float nearSize,
							 Camera* camera);

		/**
		 * How much light still contributes where the radius of the sphere cuts off
		 */
		static const float epsilon;

		GlBindShader _program;
		Eigen::Vector3f _lightPosition;
		float _lightRadius;
		float _attenuation;
		Eigen::Vector2i _screenSize;
		Texture* _color;
		Texture* _normal;
		float _nearPlaneDistance;
		float _nearPlaneSize;
		Camera* _camera;	
	};
}

#endif /* _DEFERREDPOINTLIGHT_H_ */
