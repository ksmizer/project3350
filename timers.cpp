#include "timers.h"

Timers::Timers()
{		
	physicsRate = 1.0 / 30.0;		
	oobillion = 1.0 / 1e9;	
}
double Timers::timeDiff(struct timespec *start, struct timespec *end)
{
	return (double)(end->tv_sec - start->tv_sec) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void Timers::timeCopy(struct timespec *dest, struct timespec *source)
{
	memcpy(dest, source, sizeof(struct timespec));
}
void Timers::recordTime(struct timespec *t)
{
	clock_gettime(CLOCK_REALTIME, t);
}

