#ifndef __BSD_TIMESPEC_H__
#define __BSD_TIMESPEC_H__

static inline long 
timespec_sub_sec(struct timespec t1, struct timespec t2)
{
	struct timespec diff;
	if (t2.tv_nsec-t1.tv_nsec < 0) {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
	} else {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	}
	return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

inline static unsigned int
timespec_sub_ms(struct timespec *a, struct timespec *b)
{
	return (a->tv_sec - b->tv_sec) * 1000 + (a->tv_nsec - b->tv_nsec) /
		(1000 * 1000);
}

inline static unsigned long
timespec_sub_ns(struct timespec *a, struct timespec *b)
{
	return (a->tv_sec - b->tv_sec) * 1000000000 + (a->tv_nsec - b->tv_nsec);
}

#endif
