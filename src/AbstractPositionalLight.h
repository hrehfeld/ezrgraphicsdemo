#ifndef _ABSTRACTPOSITIONALLIGHT_H_
#define _ABSTRACTPOSITIONALLIGHT_H_

#include "Light.h"
#include <Eigen/Geometry> 

namespace Ezr
{
    class AbstractPositionalLight : public Light
	{
	public:        
	    virtual ~AbstractPositionalLight() = 0;

        Eigen::Vector3f getPosition() const {return _position;}

    protected:
        AbstractPositionalLight(Eigen::Vector3f pos);
       
        Eigen::Vector3f _position;
	};
}

#endif /* _ABSTRACTPOSITIONALLIGHT_H_ */
