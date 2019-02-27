
int
compat_gettid(void)
{
	return pthread_mach_thread_np(pthread_self());
}
