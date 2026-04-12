#include "Timer.h"

Timer::Timer(float startTimeSeconds)
{
	this->startTime = startTimeSeconds;
	this->internalTime = 0;
	state = TimerState::Idle;
}

void Timer::Start()
{
	this->internalTime = startTime;
	state = TimerState::Ticking;
}

void Timer::Stop()
{
	state = TimerState::Stopped;
	this->internalTime = 0;
}

void Timer::SetIdle()
{
	state = TimerState::Idle;
}

void Timer::Pause()
{
	state = TimerState::Paused;
}

void Timer::ProcessTimer(const float dt)
{
	if (state != TimerState::Ticking)
		return;

	if (internalTime <= 0)
	{
		Stop();
		return;
	}
	
	internalTime -= dt;
}

bool Timer::IsFinished() const
{
	return internalTime <= 0 && state == TimerState::Stopped;
}