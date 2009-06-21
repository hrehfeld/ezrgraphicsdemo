#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include <Eigen/Core>

using namespace Eigen;

namespace Ezr
{
    /**
	 * Info about the current Viewport and utility functions to work with it
	 */
	class Viewport
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		Viewport(Vector2i& windowSize);
		virtual ~Viewport();
		/**
		 * Return mouse position from center in percent of window size
		 */
		Vector2f* getMousePosition(Vector2i& mousePosition);

		/**
		 * Return the window size
		 */
		inline Vector2i& getWindowSize() { return _windowSize; }

		/**
		 * Set window size
		 */
		inline void setWindowSize(Vector2i& windowSize) { _windowSize = windowSize; }
	private:
		Vector2i _windowSize;
	};
}
#endif /* _VIEWPORT_H_ */
