#include <unistd.h>
#include <syscall.h>

unsigned int
compat_gettid(void) 
{
	return syscall(SYS_gettid);
}
