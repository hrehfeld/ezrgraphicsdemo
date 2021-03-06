/*******************************************************************************
 *  Timer.cpp
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>
 ******************************************************************************/

#include "Timer.h"
#include "OpenGL.h"


namespace Ezr{

	//// TIMER //////////////////////////////////////////////////////////////
	//
	// Timer constructor
	////////////////////////////////////////////////////////////////////////
	Timer::Timer()
	{
		m_frameInterval = 0.0f;
		m_fps           = 0.0f;
	}

	Timer::~Timer()
	{
	}

	//// CALCULATE FRAME RATE //////////////////////////////////////////////
	//
	// This function calculates the current framerate as frames per second
	////////////////////////////////////////////////////////////////////////
	void Timer::CalculateFrameRate()
	{
		static float framesPerSecond    = 0.0f;		
		static float lastTime			= 0.0f;		

		static float frameTime = 0.0f;			

		float currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f;

		m_frameInterval = currentTime - frameTime;

		frameTime = currentTime;

		++framesPerSecond;

		if( currentTime - lastTime > 1.0f )
		{
			lastTime = currentTime;
			m_fps = framesPerSecond;
			framesPerSecond = 0;
		}
	}

	//// GET FRAMES PER SECOND /////////////////////////////////////////////
	//
	// This function returns the current framerate as frames per second
	////////////////////////////////////////////////////////////////////////
	const float Timer::GetFramesPerSecond() const
	{
		return m_fps;
	}

	//// GET FRAMES INTERVAL ///////////////////////////////////////////////
	//
	// This function returns the current time between two frames
	////////////////////////////////////////////////////////////////////////
	const float Timer::GetFrameInterval() const
	{
		return m_frameInterval;
	}
}
