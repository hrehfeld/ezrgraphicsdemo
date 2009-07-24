#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "OpenGL.h"
#include "glm.h"

namespace Ezr{

    class Camera;
    class Texture;

	class Scene {

	public:
		Scene(Camera* camera);
		void draw()const;

	private:
		GLMmodel* _model;
		Camera* _camera;

		Texture* _colormap;
		Texture* _normalmap;
		

	};

}

#endif
