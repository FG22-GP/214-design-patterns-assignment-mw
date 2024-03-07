#include "timer.h"

#include "gameEngine.h"

Timer::Timer(float timeInSeconds) : _timeInSeconds(timeInSeconds) {
	_currentTime = timeInSeconds;
}

Timer::~Timer() {}

const bool Timer::GetTimerFinished() const {
	return _timerFinished;
}

void Timer::ActivateTimer() {
	_timerActive = true;
}

void Timer::DeactivateTimer() {
	_timerActive = false;
	_timerFinished = false;
}

void Timer::ResetTimer() {
	_currentTime = _timeInSeconds;
	_timerFinished = false;
}

void Timer::Update() {
	if (!_timerFinished && _timerActive) {
		if (_currentTime > 0) {
			_currentTime -= deltaTime;
		} else {
			_timerFinished = true;
		}
	}
}

