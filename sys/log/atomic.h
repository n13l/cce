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
 * Logging code use stack, asynch-safe calls and atomic write() operation.
 *
 * POSIX.1 says that write(2)s of less than PIPE_BUF bytes must be atomic.
 * The precise semantics depend on whether the file descriptor is nonblocking.
 * 
 * https://tools.ietf.org/html/rfc5424
 * Asynch-safe syslog()
 *
 */

#ifndef __LOCKFREE_LOGGING_ATOMIC_H__
#define __LOCKFREE_LOGGING_ATOMIC_H__

#include <sys/compiler.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <syslog.h>

__BEGIN_DECLS

/* POSIX.1 requires PIPE_BUF to be at least 512 bytes. */
#ifndef PIPE_BUF
#define PIPE_BUF 512
#endif

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

struct log_ctx {
	const char *mod;
	const char *fn;
	const char *file;
	unsigned int secs;
	unsigned int usec;
	unsigned int line;
	unsigned int type;
	void *user;
};

#define INTERNAL_LOG_CTX_INIT(level) \
({ \
	struct log_ctx ctx = (struct log_ctx) { \
		.mod  = LOG_MODULE, .fn = __PRETTY_FUNCTION__, \
		.file = __FILE__, .secs = 0, .usec = 0, \
		.line = __LINE__, .type = level \
	}; ctx; \
})

#define INTERNAL_LOG_PRINTF_ATOMIC(type, require, fmt, ...) \
({ \
  if (log_verbose >= require) { \
    struct log_ctx log_ctx = INTERNAL_LOG_CTX_INIT(type); \
    internal_log_printf(&log_ctx, fmt, ## __VA_ARGS__); \
  } \
})

#define INTERNAL_LOG_B16_ATOMIC(type, require, buf, size) \
({ \
  if (log_verbose >= require) { \
    struct log_ctx log_ctx = INTERNAL_LOG_CTX_INIT(type); \
    internal_log_write_hex(&log_ctx, buf, size); \
  } \
})


void internal_log_printf(struct log_ctx *, const char *fmt, ...)
                         __attribute__ ((__format__ (__printf__, 2, 3)));

__END_DECLS

#endif
