#pragma once

#include <windows.h>

class Time
{
private:

	double countsPerSecond;
	double deltaTime;

	int frameCount;
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
