#pragma once

class Timer
{
private:

	//Time of the last frame (in ticks)
	__int64 m_startTime;

	//Number of CPU ticks in millisecond
	float m_ticksInMs;

	//Time taken for frame(in ms)
	float m_frameTime;

	//Time passed since game started
	__int64	m_gameTime;

public:

	/*** Constructor ***
	Destructor
	***				 ***/
	Timer();

	//Tick
	void Tick();

	/***
	//Getters
	***/

	float GetGameTime() const;
	float GetGameTimeSeconds() const;
	float GetDeltaTime() const;
	float GetDeltaTimeSeconds() const;
};

__forceinline float Timer::GetGameTime() const	
{ 
	return m_gameTime / m_ticksInMs; 
}

__forceinline float Timer::GetGameTimeSeconds() const
{
	return m_gameTime / m_ticksInMs * 0.001f; 
}

__forceinline float Timer::GetDeltaTime() const
{
	return m_frameTime; 
}

__forceinline float Timer::GetDeltaTimeSeconds() const
{
	return m_frameTime * 0.001f; 
}