#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include <Eigen/Core>

namespace Ezr
{
    /**
	 * Info about the current Viewport and utility functions to work with it
	 */
	class Viewport
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            Viewport(Eigen::Vector2i& size);
		virtual ~Viewport();
		/**
		 * Return mouse position from center in percent of window size
		 */
		Eigen::Vector2f getMousePosition(Eigen::Vector2i& mousePosition);

		/**
		 * Return the window size
		 */
		inline const Eigen::Vector2i& getViewportSize()const { return _size; }

		/**
		 * Set window size
		 */
		inline void setViewportSize(Eigen::Vector2i& size) { _size = size; }

	private:
		Eigen::Vector2i _size;
	};
}
#endif /* _VIEWPORT_H_ */
