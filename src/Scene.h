#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "glm.h"

namespace Ezr{

class Scene {

	public:
		Scene();
		void drawScene()const;

	private:
		 GLMmodel* _model;
		
		
	};

}

#endif
