#ifndef _DEFERREDRENDERER_H_
#define _DEFERREDRENDERER_H_

#include <vector>
#include <string>
#include <Eigen/Geometry>
#include "MyMath.h"
#include "shader/DeferredDrawShader.h"
#include "shader/DeferredPointLighting.h"
#include "shader/DeferredDirectionalLighting.h"

namespace Ezr
{
	class Scene;
	class Light;
	class Viewport;
    class Camera;
	class Fbo;
	
	class DeferredRenderer
	{
	public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	    DeferredRenderer(Viewport* view, Camera* camera, Scene* scene);
	    virtual ~DeferredRenderer();

		void setScene(Scene* scene);
		void setLights(std::vector<Light*>& lights);

		void draw();

	private:
		static const std::string colorBuffer;
		static const std::string normalBuffer;
        static const std::string resultBuffer;

        inline float nearPlaneSize(const float nearPlaneDist, const float fov);
        void setUniforms(GLuint program);
        void render(DirectionalLight* light);
        void render(PointLight* light);
        void render(Light* light);
        void drawPass();
		
		Scene* _scene;
		std::vector<Light*> _lights;

        Camera* _camera;
		Viewport* _view;

		Fbo* _geometryPass;
		Fbo* _lightPass;

		DeferredDrawShader _geometryShader;
		DeferredPointLighting _pointLighting;
		DeferredDirectionalLighting _directionalLighting;


		Eigen::Matrix4f _modelView;
		
	};

    /**
    * calculate smallest distance from center of the nearplane to the side
    */
    inline float DeferredRenderer::nearPlaneSize(const float nearPlaneDist, const float fov)
    {
	    float halfFov = fov / 2.0f;
	    return nearPlaneDist * tan(halfFov / 360.0f * 2.0f * MyMath::PI);
    }
}

#endif /* _DEFERREDRENDERER_H_ */
