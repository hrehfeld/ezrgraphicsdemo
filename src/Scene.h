#ifndef SCENE_H
#define SCENE_H

#include <string>

#include "glm.h"

class Scene {

public:
	Scene();

private:
	const static std::string MODELPATH;

	const GLMmodel* _model;
	
	
};

const std::string Scene::MODELPATH("res/models/Lighthouse.obj");


#endif
