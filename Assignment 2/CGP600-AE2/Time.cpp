#include "Time.h"

Time::Time()
{
	countsPerSecond = 0.0;
	deltaTime = 0.0;

	frameCount = 0;
	fps = 0;

	CounterStart = 0;
	prevDeltaTime = 0;

}

Time::~Time()
{

}

void Time::Execute()
{
	frameCount++;

	if (GetTime() > 1.0f)
	{
		fps = frameCount;
		frameCount = 0;
		StartTimer();
	}

	deltaTime = CalculateDeltaTime();

}

void Time::StartTimer()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	CounterStart = frequencyCount.QuadPart;

}

double Time::GetTime()
{
	LARGE_INTEGER currentTime;

	QueryPerformanceCounter(&currentTime);

	return double(currentTime.QuadPart - CounterStart) / countsPerSecond;
}

double Time::CalculateDeltaTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - prevDeltaTime;
	prevDeltaTime = currentTime.QuadPart;

	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}

	return float(tickCount) / countsPerSecond;
}

double Time::GetDeltaTime()
{
	return deltaTime;
}

int Time::GetFPS()
{
	return fps;
}