#pragma once
#include  <cinttypes>

enum class TimerState : std::uint8_t
{
	Stopped,
	Ticking,
	Paused,
	Idle,
};

class Timer
{
	TimerState state;
	float startTime;
	float internalTime;

public:
	Timer()
	{
		state = TimerState::Idle;
		startTime = 0;
		internalTime = 0;
	}
	explicit Timer(float startTimeSeconds);
	void Start();
	void Stop();
	void SetIdle();
	void Pause();
	void ProcessTimer(float dt);
	bool IsFinished() const;
};
