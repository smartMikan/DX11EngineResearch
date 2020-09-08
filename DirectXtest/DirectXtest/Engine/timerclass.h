#pragma once

//The TimerClass is a high precision timer that measures the exact time between frames of execution. 
//Its primary use is for synchronizing objects that require a standard time frame for movement.
//In this tutorial we won't have a use for it but we will implement it in the code so you can see how to apply it to your projects.
//The most common usage of the TimerClass is to use the frame time to figure out what percentage of a second has passed in the current frame and then move the objects by that percentage.


#include <windows.h>


// Class name: TimerClass
class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetDeltaTime();
	INT64 TimeSinceStart();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

