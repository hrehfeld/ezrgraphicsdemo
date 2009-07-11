#include "Scene.h"

#include <exception>
#include <iostream>
#include "stdlib.h"

using std::cout;
using std::cerr;
using std::endl;


namespace Ezr{

	Scene::Scene() {
		_model = glmReadOBJ("res/models/Terrain.obj");
		if (!_model){
			cerr << "couldn't load model" << endl;
			exit(0);
		}

		glmReverseWinding(_model);
		glmReverseNormals(_model);
//		glmUnitize(_model);
//		 glmFacetNormals(_model);
//		 glmVertexNormals(_model,90);
	}

	void Scene::drawScene() const
	{
		glmDraw(_model, GLM_SMOOTH | GLM_TEXTURE);
	}

}
