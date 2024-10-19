#include "speedWriter.hpp"
#include <iostream>

void SpeedWriter::addBytes(int B) {
	bytes += B;
}

void SpeedWriter::timerRun() {
	time(&startTime);
}

double SpeedWriter::getTime() {
	time(&endTime);
	seconds = difftime(endTime, startTime);
	return seconds;
}

void SpeedWriter::writeSpeedAndUpdateTime() {
	std::cout << "мегабайт в секунду : " << (double)bytes / getTime() * toMB << std::endl;
	fullTime += seconds;
	fullBytes += bytes;
	std::cout << "мегабайт в секунду ср: " << (double)fullBytes / fullTime * toMB << std::endl;
	bytes = 0;
	timerRun();
}

void SpeedWriter::timeCheckAndUpdateTime() {
	count++;
	if (count % 250 == 10) {
		if (getTime() > 3) {
			writeSpeedAndUpdateTime();
		}
	}
}