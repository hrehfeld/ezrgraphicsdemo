#ifndef POINTLIGHT_H 
#define POINTLIGHT_H 

#include "AbstractPositionalLight.h"
#include <Eigen/Geometry>

namespace Ezr{

    class PointLight : public AbstractPositionalLight{ 
    
        public: 
            PointLight(Eigen::Vector3f position, float radius, float intensity); 
            ~PointLight(); 

            float getRadius()const {return _radius;}
            float getIntensity()const {return _intensity;}
            float setRadius(float radius){_radius = radius;}
            float setIntensity(float intensity){_intensity = intensity;}
    
        private:
            float _radius;
            float _intensity;
    };    
}

#endif //POINTLIGHT_H 

