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

        void render(DirectionalLight* light);
        void render(PointLight* light);
        void render(Light* light);

	private:
		static const std::string colorBuffer;
		static const std::string normalBuffer;
        static const std::string resultBuffer;

        void setUniforms(GLuint program);
        void drawPass();

		Viewport* _view;
        Camera* _camera;
		
		
		Scene* _scene;
		std::vector<Light*> _lights;


		Fbo* _geometryPass;
		Fbo* _lightPass;

		DeferredDrawShader _geometryShader;
		DeferredPointLighting _pointLighting;
		DeferredDirectionalLighting _directionalLighting;


		Eigen::Matrix4f _modelView;
		
	};
}

#endif /* _DEFERREDRENDERER_H_ */
