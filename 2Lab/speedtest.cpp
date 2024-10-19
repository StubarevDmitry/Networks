#include "speedtest.hpp"
#include <iostream>

void SpeedTest::addBytes(int B) {
	bytes += B;
}

void SpeedTest::timerRun() {
	time(&startTime);
}

double SpeedTest::getTime() {
	time(&endTime);
	seconds = difftime(endTime, startTime);
	return seconds;
}

void SpeedTest::writeSpeedAndUpdateTime() {
	std::cout << "мегабайт в секунду : " << (double)bytes / getTime() / (double)(1024 * 1024) << std::endl;
	fullTime += seconds;
	fullBytes += bytes;
	std::cout << "мегабайт в секунду ср: " << (double)fullBytes / fullTime / (double)(1024 * 1024) << std::endl;
	bytes = 0;
	timerRun();
}

void SpeedTest::timeCheckAndUpdateTime() {
	count++;
	if (count % 250 == 10) {
		if (getTime() > 3) {
			writeSpeedAndUpdateTime();
		}
	}
}