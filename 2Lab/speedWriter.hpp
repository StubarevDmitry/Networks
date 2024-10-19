#pragma once

#include <stdio.h> 
#include <vector>
#include <time.h> 

class SpeedWriter
{
public:
	void timerRun();
	double getTime();
	void addBytes(int B);
	void writeSpeedAndUpdateTime();
	void timeCheckAndUpdateTime();

private:

	double seconds = 0;

	int fullTime = 0;

	int bytes = 0;

	int fullBytes = 0;

	int count = 0;

	time_t  startTime, endTime;
};
