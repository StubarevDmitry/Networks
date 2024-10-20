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

	//количество считанных килобайт
	int bytes = 0;

	int fullBytes = 0;

	double toMB = 1. / (double)(1024 * 1024);

	//количество заходов в функцию timeCheck()
	int count = 0;

	time_t  startTime, endTime;
};
