#include "timer.h"

Timer::Timer(const bool &startTick) {

	tickRecorded = false;

	deltaTime = std::chrono::nanoseconds(0);
	deltaTime_s = std::chrono::duration<long double>(0.0);

	timeElapsed = std::chrono::nanoseconds(0);
	timeElapsed_s = std::chrono::duration<long double>(0.0);

	if (startTick) { recordTick(); }

}

Timer::~Timer() { }

void Timer::calculateDeltaTime() {

	deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tock - tick);
	deltaTime_s = tock - tick;

}

void Timer::calculateTimeElapsed() {

	timeElapsed += deltaTime;
	timeElapsed_s += deltaTime_s;

}

void Timer::recordTick() {

	tick = std::chrono::high_resolution_clock::now();

	tickRecorded = true;

}

void Timer::recordTock() {

	tock = std::chrono::high_resolution_clock::now();

	if (tickRecorded) {

		calculateDeltaTime();
		calculateTimeElapsed();

		tickRecorded = false;

	}

}

long double Timer::getDeltaTimeSeconds() {
	return deltaTime_s.count();
}

unsigned long long int Timer::getDeltaTime() {
	return deltaTime.count();
}

long double Timer::getTimeElapsedSeconds() {
	return timeElapsed_s.count();
}

unsigned long long int Timer::getTimeElapsed() {
	return timeElapsed.count();
}

std::chrono::steady_clock::time_point& const Timer::getTick() {
	return tick;
}

std::chrono::steady_clock::time_point& const Timer::getTock() {
	return tock;
}