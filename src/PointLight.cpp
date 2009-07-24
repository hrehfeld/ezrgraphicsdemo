#include "PointLight.h"

namespace Ezr{

    PointLight::PointLight(const Eigen::Vector3f& position, float radius, float intensity) 
        : _radius(radius),
          _intensity(intensity),
          AbstractPositionalLight(position)
    {}
    
    PointLight::~PointLight()
    {}
}
