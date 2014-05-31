#include "Core.h"

#include "Timer.h"

Timer::Timer() 
	: m_gameTime{ 0 }
{
	//get CPU frequency 
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	//calculate number of ticks in one ms
	m_ticksInMs = (float)((float)freq * 0.001f);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
}

void Timer::Tick()
{
	__int64 currTime;
	float deltaTime;

	//get current time
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	//calculate delta
	deltaTime = (float)(currTime - m_startTime);

	//convert to milliseconds
	m_frameTime = deltaTime / m_ticksInMs;

	//update time of previous frame
	m_startTime = currTime;

	//sum up total time
	m_gameTime += deltaTime;
}