/*
 * Intrusive containers for single, double and circular linked list
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

#ifndef __GENERIC_LIST_H__
#define __GENERIC_LIST_H__

#include <sys/compiler.h>
#include <bsd/sort.h>

__BEGIN_DECLS

#define DECLARE_LIST(name)   struct list name
#define DEFINE_LIST(name)    DECLARE_LIST(name) = __list_init(name)
#define DEFINE_NODE(name)    struct node name = NODE_INIT

#define LIST_ITEM(item, node) &(item.node)
#define NODE_INIT            { .next = NULL, .prev = NULL } 

#define __list_init(name)    {{(struct node *)&(name), (struct node *)&(name)}}

struct node { struct node *next, *prev; };
struct list { struct node head; };

static inline void
node_init(struct node *node)
{
	node->next = node;
	node->prev = node;
}

static inline unsigned
node_added(struct node *node)
{
	return node->next != node && node->prev != node;
}

static inline void
list_init(struct list *list)
{
	struct node *head = &list->head;
	head->next = head->prev = head;
}

static inline struct node *
list_head(struct list *list)
{
	return (list->head.next != &list->head) ? list->head.next: NULL;
}

static inline struct node *
list_first(struct list *list)
{
	return list_head(list);
}

static inline struct node *
list_last(struct list *list)
{
	return (list->head.prev != &list->head) ? list->head.prev: NULL;
}

static inline struct node *
list_next(struct list *list, struct node *node)
{
	return (node->next != &list->head) ? node->next: NULL;
}

static inline struct node *
list_prev(struct list *list, struct node *node)
{
	return (node->prev != &list->head) ? node->prev: NULL;
}

static inline int
list_empty(struct list *list)
{
	return (list->head.next == &list->head);
}

static inline void
list_add_after(struct node *node, struct node *prev)
{
	prev->next->prev = node;
	node->next = prev->next;
	node->prev = prev;
	prev->next = node;
}

static inline void
list_add(struct list *list, struct node *node)
{
	struct node *head = &list->head;
	list_add_after(node, head);
}

static inline void 
list_del(struct node *node)
{
	struct node *next = node->next;
	struct node *prev = node->prev;
	next->prev = prev;
	prev->next = next;
}

static inline void
list_mov_head(struct list *list, struct node *node)
{
	list_del(node);
	list_add(list, node);
}

static inline unsigned int
list_size(struct list *list)
{
	if (list_empty(list))
		return 0;

	unsigned int size = 0;
	for (struct node *n = list->head.next; n != &list->head; n = n->next)
		size++;
	return size;
}

static inline int
list_singular(struct list *list)
{
	return (list->head.next->next == &list->head);
}

static inline int
list_order(struct list *list)
{
	int order = 0;
	if (list_empty(list) || list_singular(list))
		return order;

	struct node *n;
	for (n = list->head.next; n != &list->head; n = n->next) {
	}

	return 0;
}

static inline void
list_enable_prev(struct list *list, struct node *head)
{
	struct node *node;
	for (node = head; node->next; ) {
		node->next->prev = node;
	       	node = node->next;
	}

	list->head.prev = node;
	list->head.next = head;
	node->next = &list->head;
}

static inline struct node *
list_disable_prev(struct list *list)
{
	list->head.prev->next = NULL;
	return list->head.next;
}

#define list_move_before(x, y)   ({ list_del(x); list_add_before(x, y); })
#define list_node struct node

/* used internally */
#define __list_first(list)       ({ list_first(list); })
#define __list_next(list,x)      ({ list_next(list,x); })
#define __list_move_before(x, y) ({ list_move_before(x,y); })
#define __list_node list_node

/**
 * list_walk  - iterate over list with declared iterator
 *
 * @list:       the your list.
 * @it:	        the type safe iterator
 */

#define list_walk(self, it) \
	for ((it) = (self).head.next; ((it) != &(self).head); (it) = (it)->next)

/**
 * list_walk_delsafe  - iterate over list with declared iterator
 *
 * @list:       the your list.
 * @it:         the type safe iterator
 * @cur:        the temp node
 */

#define list_walk_delsafe(self, it, cur) \
	for ((it) = (self).head.next; ((it) != &(self).head) && \
	   ({(cur) = (it)->next;1;}); (it) = (cur))

/**
 * list_walk_reverse
 *
 * @list:       the your list.
 * @it:         the iterator
 */

#define list_walk_reverse(self, it) \
	for ((it) = (self).head.prev; ((it)!=&(self).head); (it) = (it)->prev)

/**
 * list_walk_reverse_delsafe
 *
 * @list:       the your list.
 * @it:         the iterator
 */

#define list_walk_reverse_delsafe(self, it, cur) \
	for ((it) = (self).head.prev; ((it) != &(self).head) && \
	    ({(cur) = (it)->prev;1;}); (it) = (cur))

/**
 * list_walk_reverse_cont
 *
 * @list:       the your list.
 * @it:         the iterator
 */

#define list_walk_reverse_cont(self, it) \
	for (; (it) != &self.head; (it) = (it)->prev(it))

/**
 * list_for_each - iterate over list 
 *
 * @list:       the your list.
 * @it:	        the type safe iterator
 * @type:       the structure type
 * @member:     the name of the node within the struct.
 */

#define list_for_each(self, it, type, member) \
	for (type *(it) = container_of(((self).head.next), type, member); \
	     (&(it)->member) != &((self).head); \
	     (it) = container_of((it)->member.next, type, member))

/**
 * list_for_each_delsafe - iterate over list with safety against removal
 *
 * @list:       the your list.
 * @it:         the type safe iterator 
 * @type:       the structure type
 * @member:     the name of the node within the struct.
 */

#define list_for_each_delsafe(self, it, type, member) \
	for (type *(__it), *(it) = container_of((self).head.next,type,member); \
	     (&(it)->member) != &((self).head) && \
	     ({(__it) = container_of((it)->member.next,type,member);1;}); \
	     (it) = (__it))

/**
 * list_for_each_reverse - iterate over list 
 *
 * @list:       the your list.
 * @it:	        the type safe iterator
 * @type:       the structure type
 * @member:     the name of the node within the struct.
 */

#define list_for_each_reverse(self, it, type, member) \
	for (type *(it) = container_of(((self).head.prev), type, member); \
	     (&(it)->member) != &((self).head); \
	     (it) = container_of((it)->member.prev, type, member))

/**
 * list_for_each_reverse_delsafe - iterate over list with safety against removal
 *
 * @list:       the your list.
 * @it:         the type safe iterator 
 * @type:       the structure type
 * @member:     the name of the node within the struct.
 */

#define list_for_each_reverse_delsafe(self, it, type, member) \
	for (type *(__it), *(it) = container_of((self).head.prev,type,member); \
	     (&(it)->member) != &((self).head) && \
	     ({(__it) = container_of((it)->member.prev,type,member);1;}); \
	     (it) = (__it))


/**
 * list_sort  - sort list 
 *
 * @list:       the your list.
 * @fn:	        the type safe comparator
 * @type:       the optional structure type
 * @member:     the optional name of the node within the struct.
 */

#define list_sort(list, ...) \
  va_dispatch(insert_sort_asc,__VA_ARGS__)(list,__list,__VA_ARGS__)

/**
 * list_sort_asc  - sort list 
 *
 * @list:       the your list.
 * @fn:	        the type safe comparator
 * @type:       the optional structure type
 * @member:     the optional name of the node within the struct.
 */

#define list_sort_asc(list, ...) \
  va_dispatch(insert_sort_asc,__VA_ARGS__)(list,__list,__VA_ARGS__)

/**
 * list_sort_dsc  - sort list 
 *
 * @list:       the your list.
 * @fn:	        the type safe comparator
 * @type:       the optional structure type
 * @member:     the optional name of the node within the struct.
 */

#define list_sort_dsc(list, ...) \
  va_dispatch(insert_sort_dsc,__VA_ARGS__)(list,__list, __VA_ARGS__)

__END_DECLS

#endif/*__GENERIC_LIST_H__*/
