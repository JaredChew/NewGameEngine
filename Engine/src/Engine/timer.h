#pragma once

#include <chrono>

class Timer {

private:

	bool tickRecorded;

	std::chrono::steady_clock::time_point tick;
	std::chrono::steady_clock::time_point tock;

	std::chrono::nanoseconds deltaTime;
	std::chrono::duration<double> deltaTime_s;

	std::chrono::nanoseconds timeElapsed;
	std::chrono::duration<double> timeElapsed_s;

private:

	void calculateDeltaTime();
	void calculateTimeElapsed();

public:

	Timer(const bool &startTick = false);
	~Timer();

	void recordTick();
	void recordTock();

	long double getDeltaTimeSeconds();
	unsigned long long int getDeltaTime();

	long double getTimeElapsedSeconds();
	unsigned long long int getTimeElapsed();

	std::chrono::steady_clock::time_point& const getTick();
	std::chrono::steady_clock::time_point& const getTock();

};