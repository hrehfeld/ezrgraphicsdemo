#include "Scene.h"

#include <exception>
#include <iostream>
#include "stdlib.h"

using std::cout;
using std::cerr;
using std::endl;

Scene::Scene() {
	_model = glmReadOBJ(const_cast<char*> (MODELPATH.c_str()));
	if (!_model){
		cerr << "couldn't load model" << endl;
		exit(0);
	}
}
