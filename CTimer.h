//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef _CTIMER_H_
#define _CTIMER_H_

#include <Windows.h>
#include <cmath>
#include <tchar.h>
#include <time.h>

const ULONG MAX_SAMPLE_COUNT = 50; //Maximum frame time sample count

//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CTimer (Class)
// Desc : Game Timer class, queries performance hardware if available, and 
//        calculates all the various values required for frame rate based
//        vector / value scaling.
//-----------------------------------------------------------------------------

class CTimer
{
public: 
	
	CTimer();
	virtual ~CTimer();

	void			Tick(float LockFPS = 0.0f);
	unsigned long	GetFrameRate(LPTSTR lpszString = NULL) const;
	float			GetTimeElapsed() const;

private:
	bool			m_PerfHardware;			//Has performance counter
	float			m_TimeScale;			//Amount to scale counter
	float			m_TimeElapsed;			//Since previous frame
	__int64			m_CurrentTime;			//Current performance counter
	__int64			m_LastTime;				//Performance counter last frame
	__int64			m_PerfFreq;				//Performance Frequency

	float			m_FrameTime[MAX_SAMPLE_COUNT];
	ULONG			m_SampleCount;

	unsigned long	m_FrameRate;			//Stores current framerate
	unsigned long	m_FPSFrameCount;		//Elapsed frames in any given second
	float			m_FPSTimeElapsed;		//How much time has passed during FPS sample
};

#endif //_CTIMER_H_
