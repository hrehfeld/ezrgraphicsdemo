#include "DirectionalLight.h"

using namespace Eigen;


namespace Ezr
{
	
	
	DirectionalLight::DirectionalLight(const Vector3f& direction, float intensity)
		: _direction(direction)
		  //copy
		, _intensity(intensity)
	{
	}


	DirectionalLight::~DirectionalLight()
	{
	}
}
	
