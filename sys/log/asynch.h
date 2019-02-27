/*
 * Lock-free and asynch-safe logging capability
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2018                          Daniel Kubec <n13l@rtfm.cz>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"),to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in   
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Logging code use stack, asynch-safe calls and atomic write() operation.
 *
 * POSIX.1 says that write(2)s of less than PIPE_BUF bytes must be atomic.
 * The precise semantics depend on whether the file descriptor is nonblocking.
 * 
 * https://tools.ietf.org/html/rfc5424
 * Asynch-safe syslog()
 *
 * Logging code support for outputting syslog, just uses the regular C library 
 * syslog() function. The problem is that this function is not async signal safe, 
 * mostly because of its use of the printf family of functions internally. 
 * Annoyingly libvirt does not even need printf support here, because it has 
 * already expanded the log message string.
 *
 * Following log line if LOG_CAP_TIME is defined:
 *
 *	 +- month           +- process id
 *	 |  +- day          |      +- thread id      +- message
 *	 |  |               |      |                 |
 *	 04-29 22:43:20.244 1000  1000 I decodeTLS Number of arguments: 1
 */

#ifndef __LOCKFREE_LOGGING_ASYNCH_H__
#define __LOCKFREE_LOGGING_ASYNCH_H__

#include <sys/compiler.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <syslog.h>

__BEGIN_DECLS

/*
 * https://tools.ietf.org/html/rfc5424
 *
 * TODO: asynchsafe syslog()
 *
 * logging code support for outputting syslog, just uses the regular C library 
 * syslog() function. The problem is that this function is not async signal safe, 
 * mostly because of its use of the printf family of functions internally. 
 * Annoyingly libvirt does not even need printf support here, because it has 
 * already expanded the log message string.
 *
 */

#ifndef LOG_ERROR
#define LOG_ERROR 3
#endif
#ifndef LOG_WARN
#define LOG_WARN 4
#endif
#ifndef LOG_INFO
#define LOG_INFO 6
#endif
#ifndef LOG_DEBUG
#define LOG_DEBUG 7
#endif
#ifndef LOG_DEBUG1
#define LOG_DEBUG1 8
#endif
#ifndef LOG_DEBUG2
#define LOG_DEBUG2 9
#endif
#ifndef LOG_DEBUG3
#define LOG_DEBUG3 10
#endif
#ifndef LOG_DEBUG4
#define LOG_DEBUG4 11
#endif

#ifndef LOG_MODULE
#define LOG_MODULE ""
#endif

__END_DECLS

#endif
