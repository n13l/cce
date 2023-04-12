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
#include <bsd/tailq.h>
#include <math.h>
#include <limits.h>

__BEGIN_DECLS

#define DECLARE_HASHTABLE(name, bits) \
	struct tailq name[1 << (bits)]
#define DEFINE_HASHTABLE(name, bits) DECLARE_HASHTABLE(name, bits) = { \
		[0 ... ((1<<(bits))-1)] = init_tailq \
	}

#define hash_init_table(name, bits) \
({ \
	for (unsigned i = 0; i < (1 << bits); i++) \
		name[i] = init_tailq; \
})

#define hash_bits(name) (unsigned)(log2(array_size(name)))
#define hash_add(table, member, hash) tailq_add(&(table[hash]),member) 
#define hash_seq(seqno, bits) ({(seqno & ((1 << bits) - 1));})

/* (RUC) recently-used-cache */
#define hash_ruc(table, node, hash) { \
	tailq_del(node); tailq_add(&(table[hash]),node); \
}

#define hash_del(node) tailq_del(node);
#define hash_del_init(node) tailq_del_init(node);

#define hash_empty(table, hash) tailq_empty(&(table[hash]))
#define hash_init(node) (node) = init_qnode

#define hash_for_each(table, hash, it, type, member) \
	tailq_for_each((table[hash]), it, type, member)

#define hash_for_each_delsafe(table, hash, it, type, member) \
	tailq_for_each_delsafe((table[hash]), it, type, member)

__END_DECLS

#endif
