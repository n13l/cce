/*
 * Since Linux 2.6.11, the pipe capacity is 16 pages (i.e., 65,536 bytes in a 
 * system with a page size of 4096 bytes). Since Linux 2.6.35, the default pipe 
 * capacity is 16 pages, but the capacity can be queried and set using the 
 * fcntl(2) F_GETPIPE_SZ and F_SETPIPE_SZ operations.
 */
