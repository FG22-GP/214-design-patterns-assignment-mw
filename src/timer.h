#pragma once

class Timer {
public:
	Timer(float timeInSeconds);
	~Timer();

	const bool GetTimerFinished() const;

	void ActivateTimer();
	void DeactivateTimer();
	void ResetTimer();
	void Update();

private:
	bool _timerActive = true;
	bool _timerFinished = false;

	const float _timeInSeconds;	
	float _currentTime;

};