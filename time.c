#ifndef TIME
#define TIME

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

// returns time seed
time_t get_time_seed() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return tv.tv_sec;
}

// returns time at either start or end
time_t timing() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return tv.tv_usec;
}

#endif
