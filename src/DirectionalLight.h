#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

#include "Light.h"

#include <Eigen/Geometry>

namespace Ezr
{
	class DirectionalLight : public Light
	{
	public:
	    DirectionalLight(const Eigen::Vector3f& direction, float intensity);
	    virtual ~DirectionalLight();

		const Eigen::Vector3f& getDirection() const { return _direction; } 
		float getIntesity() const { return _intensity; } 

	private:
		Eigen::Vector3f _direction;
		float _intensity;
	};
}

#endif /* _DIRECTIONALLIGHT_H_ */
