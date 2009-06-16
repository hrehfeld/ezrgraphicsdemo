/*******************************************************************************
 *  Timer.h
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/

#ifndef _TIMER_H
#define _TIMER_H

namespace Ezr{

	class Timer {

	public:

		Timer();
		~Timer();
		void CalculateFrameRate();
		const float GetFramesPerSecond() const;
		const float GetFrameInterval() const;

	private:

		float m_frameInterval;
		float m_fps;
	};
}

#endif
