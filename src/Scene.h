#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "OpenGL.h"
#include "glm.h"
#include "DeferredDirectionalLight.h"
#include "DeferredPointLight.h"
#include <vector>

namespace Ezr{

class Scene {
	public:
        typedef std::vector<DeferredDirectionalLight::DirectionalLightPtr> DirectionalLightList; 
		typedef std::vector<DeferredPointLight::PointLightPtr> PointLightList;
        typedef std::vector<DeferredDirectionalLight::DirectionalLightPtr>::iterator DirectionalLightListIterator; 
        typedef std::vector<DeferredPointLight::PointLightPtr>::iterator PointLightListIterator;
		
        Scene();

        void drawScene()const;

        inline void addDirectionalLight(
            const DeferredDirectionalLight::DirectionalLightPtr& light);

        inline void addPointLight(
            const DeferredPointLight::PointLightPtr& light);

        inline void setDiretionalLights(
            const DirectionalLightList& directionalLights);

        inline void setPointLights(
            const PointLightList& pointLights);

        inline const DirectionalLightList& 
            getDirectionalLights() const;

        inline const PointLightList& 
            getPointLights() const;

	private:
		 GLMmodel* _model;
         DirectionalLightList _directionalLights;
         PointLightList _pointLights;
	};

inline void Scene::addDirectionalLight(
        const DeferredDirectionalLight::DirectionalLightPtr& light)
    {
        _directionalLights.push_back(light);
    }
    
    inline void Scene::addPointLight(
        const DeferredPointLight::PointLightPtr& light)
    {
        _pointLights.push_back(light);
    }

    inline void Scene::setDiretionalLights(
        const Scene::DirectionalLightList& directionalLights)
    {
        _directionalLights = directionalLights;
    }

    inline void Scene::setPointLights(
        const PointLightList& pointLights)
    {
        _pointLights = pointLights;
    }

    inline const Scene::DirectionalLightList& 
        Scene::getDirectionalLights() const
    {
        return _directionalLights;
    }

    inline const Scene::PointLightList& 
        Scene::getPointLights() const
    {
        return _pointLights;
    }
}

#endif
