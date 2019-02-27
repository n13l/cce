#include <sys/compiler.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>

u64
cpu_get_cycles(void) {                                                 
	unsigned int x, y;                                                      
	__asm__ __volatile__ ("rdtsc" : "=a" (x), "=d" (y));                    
	return ((u64)y << 32) | x;                                         
}

int
cpu_get_time(struct timespec *x)
{
	return clock_gettime(CLOCK_PROCESS_CPUTIME_ID, x);
}
