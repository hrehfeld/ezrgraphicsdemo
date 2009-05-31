/*******************************************************************************
 *  Time.h
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/

#ifndef _TIME_H
#define _TIME_H

namespace Ezr{

	class Time {

	public:

		Time();
		~Time();
		void CalculateFrameRate();
		const float GetFramesPerSecond() const;
		const float GetFrameInterval() const;

	private:

		float m_frameInterval;
		float m_fps;
	};
}

#endif