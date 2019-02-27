#include <sys/compiler.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
/*
#include <asm/cachectl.h>
*/
u64
cpu_clocks_per_sec(void)
{
	return CLOCKS_PER_SEC;
}

/*
void
cpu_cache_flush(void)
{
	cacheflush(NULL, 0, ICACHE | DCACHE);
}
*/

/*
 * To free pagecache:
 *   echo 1 > /proc/sys/vm/drop_caches
*/

void
page_cache_flush(void)
{
	FILE *fd = fopen ("/proc/sys/vm/drop_caches", "w"); 
	if (!fd)
		return;
	fwrite("1", 1, 1, fd);
	fclose(fd);	
}

/*
 * To free pagecache, dentries and inodes:
 *   echo 3 > /proc/sys/vm/drop_caches
 */

void
page_cache_flush_all(void)
{
	FILE *fd = fopen ("/proc/sys/vm/drop_caches", "w"); 
	if (!fd)
		return;
	fwrite("3", 1, 1, fd);
	fclose(fd);	
}
