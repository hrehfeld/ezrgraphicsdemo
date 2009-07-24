#include "AbstractPositionalLight.h"

namespace Ezr{

    AbstractPositionalLight::AbstractPositionalLight(Eigen::Vector3f pos)
        : _position(pos)
    {}
    
    AbstractPositionalLight::~AbstractPositionalLight() 
    {}
}