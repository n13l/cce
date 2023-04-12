/*
 * Generic hash functions 
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 - 2019                        Daniel Kubec <niel@rtfm.cz>
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **/

#ifndef __GENERIC_HASH_FN_H__
#define __GENERIC_HASH_FN_H__

#include <sys/compiler.h>
#include <sys/cpu.h>
#include <math.h>
#include <limits.h>

#ifndef XXH_INLINE_ALL
#define XXH_INLINE_ALL
#endif
#include "xxhash.h"

static inline u64
hash_u64(u64 x, unsigned int bits)
{
	x = (x ^ (x >> 30)) * (u64)(0xbf58476d1ce4e5b9);
	x = (x ^ (x >> 27)) * (u64)(0x94d049bb133111eb);
	x = x ^ (x >> 31);
	return x >> (64 - bits);
}

static inline u32
hash_u32(u32 x, unsigned int bits)
{
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = (x >> 16) ^ x;
	return x >> (32 - bits);
}

static inline unsigned int
hash_seqence(unsigned int seqno, unsigned int bits)
{
	return (seqno & ((1 << bits) - 1));
}

/*
 * http://www.burtleburtle.net/bob/hash/doobs.html
 *
 * Bernstein's hash
 *
 * If your keys are lowercase English words, this will fit 6 characters into a 
 * 32-bit hash with no collisions (you'd have to compare all 32 bits). If your 
 * keys are mixed case English words, 65*hash+key[i] fits 5 characters into a 
 * 32-bit hash with no collisions. That means this type of hash can produce 
 * (for the right type of keys) fewer collisions than a hash that gives a more 
 * truly random distribution. If your platform doesn't have fast multiplies, 
 * no sweat, 33*hash = hash+(hash<<5) and most compilers will figure that out 
 * for you.
 *
 * On the down side, if you don't have short text keys, this hash has a easily 
 * detectable flaws. For example, there's a 3-into-2 funnel that 0x0021 and 
 * 0x0100 both have the same hash (hex 0x21, decimal 33)
 */

static inline u32
hash_u32_bernstein(u8 *key, u32 len, u32 level)
{
	u32 i, hash = level;
	for (i = 0; i < len; ++i) hash = 33 * hash + key[i];
		return hash;
}

static inline unsigned long
hash_ptr(void *ptr, unsigned int bits)
{
#if CPU_ARCH_BITS == 32
	return (unsigned long)hash_u32((u32)ptr, bits);
#elif CPU_ARCH_BITS == 64
	return (unsigned long)hash_u64((u64)ptr, bits);
#endif
}

static inline unsigned long long
hash_buffer(const u8 *ptr, unsigned int size)
{
    unsigned long long const seed = 0;
#if CPU_ARCH_BITS == 32
    unsigned long long const hash = XXH32(ptr, size, seed);
#elif CPU_ARCH_BITS == 64
    unsigned long long const hash = XXH64(ptr, size, seed);
#endif
    return hash;
}

static inline u32
hash_buffer_u32(const u8 *ptr, unsigned int size, unsigned int bits)
{
	return ((u32)hash_buffer(ptr, size)) >> (32 - bits);
}

static inline unsigned long long
hash_string(const char *str)
{
	size_t size = strlen(str);
	return hash_buffer((u8*)str, size);
}

/* 
 * https://arxiv.org/abs/1503.03465
 * Faster 64-bit universal hashing using carry-less multiplications
 *
 * Daniel Lemire, Owen Kaser
 * (Submitted on 11 Mar 2015 (v1), last revised 4 Nov 2015 (this version, v8))
 * Intel and AMD support the Carry-less Multiplication (CLMUL) instruction set 
 * in their x64 processors. We use CLMUL to implement an almost universal 
 * 64-bit hash family (CLHASH). We compare this new family with what might be 
 * the fastest almost universal family on x64 processors (VHASH). We find that 
 * CLHASH is at least 60% faster. We also compare CLHASH with a popular hash 
 * function designed for speed (Google's CityHash). We find that CLHASH is 40% 
 * faster than CityHash on inputs larger than 64 bytes and just as fast 
 * otherwise.
 */

#endif
