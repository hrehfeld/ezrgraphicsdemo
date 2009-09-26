#ifndef _DEFERREDDIRECTIONALLIGHT_H_
#define _DEFERREDDIRECTIONALLIGHT_H_

#include "Shader.h"
#include <string>
#include "gl/GlBindShader.h"
#include <boost/shared_ptr.hpp>
#include <Eigen/Geometry>

namespace Ezr
{
    class Texture;
	class Camera;

	class DeferredDirectionalLight : public Shader
	{
	public:
        typedef boost::shared_ptr<DeferredDirectionalLight> DirectionalLightPtr;
	    virtual ~DeferredDirectionalLight();

		virtual void bind();
		virtual void unbind();

        inline void setColorBuffer(Texture* color){_color = color;}
        inline void setNormalBuffer(Texture* normal){_normal = normal;}

        static DirectionalLightPtr create(const Eigen::Vector3f& lightDirection,
									      float nearPlaneDistance,
									      float nearSize,
									      Camera* camera);
		
	private:
		static const std::string vertexShaderPath;
		static const std::string fragmentShaderPath;

        DeferredDirectionalLight(const Eigen::Vector3f& lightDirection,
                                    float nearPlaneDistance,
									float nearSize,
									Camera* camera);

		GlBindShader _program;
		Eigen::Vector3f _lightDirection;
		Texture* _color;
		Texture* _normal;
		float _nearPlaneDistance;
		float _nearPlaneSize;
		Camera* _camera;		
	};
}

#endif /* _DEFERREDDIRECTIONALLIGHT_H_ */
