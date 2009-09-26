#include "Viewport.h"
#include <Eigen/Geometry>

using namespace Eigen;

namespace Ezr
{

	Viewport::Viewport(Vector2i& size) : _size(size)
	{
	}
	
	Vector2f Viewport::getMousePosition(Vector2i& mousePosition)
	{
		Vector2i half = _size / 2;

		Vector2i relativeMousePos = mousePosition - half;

		return Vector2f(static_cast<float>(relativeMousePos.x()) / static_cast<float>(half.x()),
							static_cast<float>(relativeMousePos.y()) / static_cast<float>(half.y()));
	}

	Viewport::~Viewport()
	{
	}
	
}
