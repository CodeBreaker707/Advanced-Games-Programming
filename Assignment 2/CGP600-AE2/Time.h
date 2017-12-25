#pragma once

#include <windows.h>

class Time
{
private:

	
	double countsPerSecond;

	// Time between each tick
	double deltaTime;

	// Frame Count every tick
	int frameCount;

	// Frames per second
	int fps;

	__int64 CounterStart;
	__int64 prevDeltaTime;

public:
	Time();
	~Time();

	void StartTimer();

	double GetTime();

	double CalculateDeltaTime();

	void Execute();

	double GetDeltaTime();
	int GetFPS();

};
