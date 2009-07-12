#include "PointLight.h"

#include "DeferredRenderer.h"

namespace Ezr{

    PointLight::PointLight(const Eigen::Vector3f& position, float radius, float intensity) 
        : _radius(radius),
          _intensity(intensity),
          AbstractPositionalLight(position)
    {}
    
    PointLight::~PointLight()
    {}

	void PointLight::renderLight(DeferredRenderer* r) { r->render(this); }
}
