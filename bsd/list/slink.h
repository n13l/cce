/*
 * Intrusive single linked list
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

#ifndef __GENERIC_SINGLE_LINKED_LIST_H__
#define __GENERIC_SINGLE_LINKED_LIST_H__

#include <sys/compiler.h>

__BEGIN_DECLS

struct snode { struct snode *next; };
struct slist { struct snode *head; };

static inline void
__snode_init(struct snode *snode)
{
	snode->next = NULL;
}

static inline struct snode*
__slist_head(struct slist *slist)
{
	return slist->head;
}

static inline struct snode *
__slist_begin(struct slist *slist)
{
	return slist->head;
}

static inline struct snode *
__slist_end(struct snode *snode)
{
	return snode->next ? NULL: snode;
}

static inline struct snode *
__slist_first(struct slist *slist)
{
	return slist->head;
}

static inline struct snode *
__slist_next(struct snode *snode)
{
	return snode->next;
}


static inline void
__slist_add(struct snode *snode, struct snode *after)
{
	after->next = snode;
}

static inline void
__slist_del(struct snode *node, struct snode *prev)
{
	if (!prev)
		return;
	prev->next = node->next;
}

#define __slist_entry(node, type, member) \
	((type *)((char *)node - offsetof(type, member)))

/**
 * snode_split - split single-linked list 
 * @head:        the list.
 */

#define snode_split(head) \
({ \
	__typeof__(*head) *node, *fast, *slow; \
	for (fast = slow = head; fast->next && fast->next->next; ) { \
		fast = fast->next->next; slow = slow->next; \
	} \
	node = slow->next; slow->next = NULL; node; \
})

/**
 * snode_walk  - iterate over nodes with declared iterator
 *
 * @node        the node with next relation, null terminated
 * @it:	        the type safe iterator
 * @member:     the optional name of the node within the struct.
 */

#define snode_walk(node, ...) \
	va_dispatch(snode_walk,__VA_ARGS__)(node,__VA_ARGS__)
#define snode_walk1(node, it) \
	for ((it) = (node); (it); (it) = (it)->next)

/**
 * slist_walk  - iterate over list with declared iterator
 *
 * @list:       the your list.
 * @it:	        the type safe iterator
 * @member:     the optional name of the node within the struct.
 */

#define slist_walk(list, ...) \
	va_dispatch(slist_walk,__VA_ARGS__)(list,__VA_ARGS__)
#define slist_walk1(list, it) \
	for ((it) = NODE_HEAD(list); (it); (it) = (it)->next)

/**
 * slist_walk_next - iterate over list with existing iterator
 *
 * @it:	        the type safe iterator
 * @member:     the optional name of the node within the struct.
 */

#define slist_walk_next(list, ...) \
	va_dispatch(list_walk_next,__VA_ARGS__)(list,__VA_ARGS__)
#define slist_walk_next1(list, it) \
	for ((it) = (it)->next; (it); (it) = (it)->next)

#define slist_for_each(item, member, it) \
	for (; item; item = it)

#define slist_for_each_delsafe(item, member, it) \
	for (; item && ({it=(__typeof__(item))item->member.next;1;}); item = it)

__END_DECLS

#endif/*__GENERIC_LIST_H__*/
