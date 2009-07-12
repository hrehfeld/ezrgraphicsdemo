#include "DirectionalLight.h"
#include "DeferredRenderer.h"

using namespace Eigen;


namespace Ezr
{
	
	
	DirectionalLight::DirectionalLight(const Vector3f& direction, float intensity)
		: _direction(direction), 
        _intensity(intensity)
	{
	}


	DirectionalLight::~DirectionalLight()
	{
	}
	
	void DirectionalLight::renderLight(DeferredRenderer* r) { r->render(this); }
}
	
