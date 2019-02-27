/*
 * Intrusive tailque
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

#ifndef __BSD_INTRUSIVE_TAILQ_H__
#define __BSD_INTRUSIVE_TAILQ_H__

#include <sys/compiler.h>

__BEGIN_DECLS

struct qnode { struct qnode *next, **prev; };
struct tailq { struct qnode *head; };

#define TAILQ_INIT (struct tailq) { .head = NULL }
#define TAILQ_INIT_HEAD(name) { &(name), &(name) }
#define TAILQ_INIT_NODE (struct qnode) {.next = NULL, .prev = NULL}
#define TAILQ_HEAD(tailq) ({ tailq->head; })
#define TAILQ_NEXT(node) ({ node->next; })

#define TAILQ_ITER_DELSAFE(it, it_next) \
	((it)&&({(it_next)=(it)->next;1;}))
#define TAILQ_HEAD_DELSAFE(tailq, it) \
	({ it = (tailq).head; NULL; })
#define TAILQ_HEAD_TYPE_DELSAFE(tailq, it, member) \
	({it = __container_of_safe((tailq)->head, typeof(*it), member); NULL; })
#define TAILQ_NEXT_TYPE(it_next, type, member) \
	__container_of_safe(it_next, type, member)
#define TAILQ_ITER_TYPE_DELSAFE(it, it_next, type, member) \
	((it)&&({(it_next)=(it)->member.next;1;}))

static inline void 
__tailq_add(struct tailq *tailq, struct qnode *qnode)
{
	struct qnode *head = tailq->head;
	qnode->next = head;
	if (head)
		head->prev = &qnode->next;
	tailq->head = qnode;
	qnode->prev = &tailq->head;
}

static inline void 
__tailq_del(struct qnode *qnode)
{
	if (!qnode->prev)
		return;
	
	struct qnode *next  = qnode->next;
	struct qnode **prev = qnode->prev;
	*prev = next;
	if (next)
		next->prev = prev;

	*qnode = TAILQ_INIT_NODE;
}

static inline void
__tailq_add_before(struct qnode *qnode, struct qnode *next)
{
	qnode->prev = next->prev;
	qnode->next = next;
	next->prev = &qnode->next;
	*(qnode->prev) = qnode;
}

static inline void
__tailq_add_after(struct qnode *qnode, struct qnode *next)
{
	next->next = qnode->next;
	qnode->next = next;
	next->prev = &qnode->next;

	if(next->next)
		next->next->prev  = &next->next;
}

static inline int
__tailq_singular(struct tailq *tailq, struct qnode *qnode)
{
	return !qnode->next && qnode->prev == &tailq->head;
}

static inline int
__tailq_empty(const struct tailq *tailq)
{
	return !tailq->head;
}

/**
 * tailq_for_each
 *
 * @self:      the tailq instance
 * @iter:      iterator
 * @type:      the structure type
 * @member:    the name of the node within the struct.
 */

#define tailq_for_each(self, iter, type, member) \
  for (type *iter = container_of_safe((self)->head, type, member); (iter); \
    iter = container_of_safe((iter)->member.next, type, member))


#ifdef CONFIG_URCU

#include <urcu/arch.h>
#include <urcu-pointer.h>

#define tailq_first_rcu(head)	(*((struct qnode **)(&(head)->head)))
#define tailq_next_rcu(node)	(*((struct qnode **)(&(node)->next)))
#define tailq_pprev_rcu(node)	(*((struct qnode **)((node)->pprev)))

static inline void 
__tailq_add_rcu(struct qnode *qnode, struct tailq *tailq)
{
	struct qnode *head = tailq->head;
	qnode->next = head; qnode->pprev = &h->head;
	if (head)
		head->pprev = &n->next;
	rcu_assign_pointer(tailq_first_rcu(head), qnode);
}

static inline void
__tailq_del_rcu(struct qnode *qnode)
{
	if (qnode->next)
		qnode->next->prev = qnode->prev;
	CMM_STORE_SHARED(qnode->prev->next, qnode->next);
}

static inline void
__tailq_add_before_rcu(struct qnode *node, struct qnode *next)
{
	node->pprev = next->pprev;
	node->next = next;
	rcu_assign_pointer(tailq_pprev_rcu(node), node);
	next->pprev = &node->next;
}

static inline void 
__tailq_add_after_rcu(struct qnode *node, struct qnode *prev)
{
	node->next = prev->next;
	node->pprev = &prev->next;
	rcu_assign_pointer(tailq_next_rcu(prev), node);
	if (node->next)
		node->next->pprev = &node->next;
}

#endif/*CONFIG_URCU*/

__END_DECLS

#endif/*__GENERIC_TAILQ_H__*/
