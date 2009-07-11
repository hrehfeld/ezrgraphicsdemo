#ifndef _DEFERREDRENDERER_H_
#define _DEFERREDRENDERER_H_

#include <vector>
#include <string>

#include "shader/DeferredDrawShader.h"
#include "shader/DeferredPointLighting.h"
#include "shader/DeferredDirectionalLighting.h"

namespace Ezr
{

	class Scene;
	class Light;
	class Viewport;
	class Fbo;
	
	class DeferredRenderer
	{
	public:
	    DeferredRenderer(Viewport* view, Camera* camera);
	    virtual ~DeferredRenderer();

		void setScene(Scene* scene);
		void setLights(std::vector<Light*>& lights);

		void draw();

	private:
		static const std::string colorBuffer;
		static const std::string normalBuffer;
		
		Scene* _scene;
		std::vector<Light*> _lights;

		Viewport* _view;

		Fbo* _geometryPass;
		Fbo* _lightPass;

		DeferredDrawShader _geometryShader;
		DeferredPointLighting _pointLighting;
		DeferredDirectionalLighting _directionalLighting;


		Eigen::Matrix4f _modelView;
		
	};
}

#endif /* _DEFERREDRENDERER_H_ */
