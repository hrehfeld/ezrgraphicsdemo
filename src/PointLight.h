#ifndef POINTLIGHT_H 
#define POINTLIGHT_H 

#include "AbstractPositionalLight.h"
#include <Eigen/Geometry>

namespace Ezr{

	class DeferredRenderer;
 
    class PointLight : public AbstractPositionalLight{ 
		
	public: 
		PointLight(const Eigen::Vector3f& position, float radius, float intensity); 
		~PointLight(); 

		float getRadius()const {return _radius;}
		float getIntensity()const {return _intensity;}
		void setRadius(float radius){_radius = radius;}
		void setIntensity(float intensity){_intensity = intensity;}

		virtual void renderLight(DeferredRenderer* r);
	private:
		float _radius;
		float _intensity;
    };    
}

#endif //POINTLIGHT_H 

