#include <time.h>
#include <stdio.h>
#include <cstring>

#ifndef TIMERS_H
#define TIMERS_H

class Timers
{
	public:
	double physicsRate, oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	Timers();
	double timeDiff(struct timespec *start, struct timespec *end);
	void timeCopy(struct timespec *dest, struct timespec *source);
	void recordTime(struct timespec *t);
};

#endif
