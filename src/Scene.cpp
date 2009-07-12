#include "Scene.h"

#include <exception>
#include <iostream>
#include "stdlib.h"

using std::cout;
using std::cerr;
using std::endl;


namespace Ezr{

	Scene::Scene(Camera* camera)
		: _camera(camera)
		, _colorMap("res/textures/lava.tga")
		, _normalMap("res/textures/lava-normal.tga")
	{
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

	void Scene::draw() const
	{
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
		
		cam->CamLookAt();

		glActiveTexture(GL_TEXTURE0);
		_colorMap->bind();
        glEnable(GL_TEXTURE_2D);

		// //multitexturing
		glActiveTexture(GL_TEXTURE1);
		_normalMap->bind();
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glColor4f(1,0,0,1);
		glutSolidTeapot(1);

		glColor4f(1,1,1,1);
		glmDraw(_model, GLM_SMOOTH | GLM_TEXTURE);

		glDisable(GL_CULL_FACE);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		
	}

}
