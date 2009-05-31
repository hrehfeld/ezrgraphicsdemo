#include "Scene.h"

#include <exception>
#include <iostream>
#include "stdlib.h"

using std::cout;
using std::cerr;
using std::endl;


namespace Ezr{

	Scene::Scene() {
		_model = glmReadOBJ("res/models/Lighthouse.obj");
		if (!_model){
			cerr << "couldn't load model" << endl;
			exit(0);
		}
	}

	void Scene::drawScene() const
	{
		glmUnitize(_model);
		glmFacetNormals(_model);
		glmVertexNormals(_model,90);
		glmDraw(_model, GLM_SMOOTH | GLM_MATERIAL);
	}

}