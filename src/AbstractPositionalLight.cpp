#include "AbstractPositionalLight.h"

namespace Ezr{

    AbstractPositionalLight::AbstractPositionalLight(const Eigen::Vector3f& pos)
        : _position(pos)
    {}
    
    AbstractPositionalLight::~AbstractPositionalLight() 
    {}
}