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

#ifndef __GENERIC_HASH_TABLE_H__
#define __GENERIC_HASH_TABLE_H__

#include <sys/compiler.h>
#include <sys/cpu.h>
#include <bsd/list/tailq.h>
#include <bsd/tailq.h>
#include <math.h>
#include <limits.h>

__BEGIN_DECLS

#define DEFINE_HASHTABLE(name, bits) \
	struct tailq name[1 << (bits)] = {[0 ... ((1<<(bits))-1)] = TAILQ_INIT}

#define hash_bits(name) (unsigned int)(log2(array_size(name)))
#define hash_entries(name) array_size(name)

#define hash_data(name, key) (u32)hash_u32(key, hash_bits(name))
#define hash_skey(name, key) (u32)hash_u32(hash_string(key), hash_bits(name))
#define hash_sbuf(name, key, len) (u32)hash_u32(hash_buffer(key, len), hash_bits(name))

#define hash_add(table, member, slot) tailq_add(&table[slot],member)
#define hash_del(node) tailq_del(node);
#define hash_get(table, key) &name[hash_data(key, hash_bits(name))]

#define hash_empty(table, slot) tailq_empty(&table[slot])

#define hash_init(node) (node) = TAILQ_INIT_NODE

#define hash_for_each(table, slot, it, type, member) \
	tailq_for_each(&table[slot], it, type, member)

__END_DECLS

#endif
