/*
 * Lock-free and asynch-safe logging trace functions
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
 */

#ifndef __LOCKFREE_LOGGING_TRACE_H__
#define __LOCKFREE_LOGGING_TRACE_H__

#include <sys/compiler.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

__BEGIN_DECLS

#ifdef CONFIG_DEBUG
#define trace1(fmt, ...) \
  __log_write_atomic_helper(LOG_DEBUG1, 1, fmt, ## __VA_ARGS__)
#define trace2(fmt, ...) \
  __log_write_atomic_helper(LOG_DEBUG2, 2, fmt, ## __VA_ARGS__)
#define trace3(fmt, ...) \
  __log_write_atomic_helper(LOG_DEBUG3, 3, fmt, ## __VA_ARGS__)
#define trace4(fmt, ...) \
  __log_write_atomic_helper(LOG_DEBUG4, 4, fmt, ## __VA_ARGS__)
#else
#define trace1(fmt, ...) do {} while(0)
#define trace2(fmt, ...) do {} while(0)
#define trace3(fmt, ...) do {} while(0)
#define trace4(fmt, ...) do {} while(0)
#endif/*CONFIG_DEBUG*/

__END_DECLS

#endif
