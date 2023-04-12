/*
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

#ifndef __GENERIC_TAILQ_H__
#define __GENERIC_TAILQ_H__

#include <sys/compiler.h>

__BEGIN_DECLS

struct qnode { struct qnode *next, **prev; };
struct tailq { struct qnode *head; };

#define DEFINE_TAILQ(name) struct tailq name = { .head = NULL }

#define init_qnode (struct qnode) { .next = NULL, .prev = NULL}
#define init_tailq (struct tailq) { .head = NULL }

static inline void
qnode_init(struct qnode *qnode)
{
	qnode->next = NULL;
	qnode->prev = NULL;
}

static inline void
tailq_add_head(struct tailq *tailq, struct qnode *qnode)
{
	struct qnode *head = tailq->head;
	qnode->next = head;
	if (head)
		head->prev = &qnode->next;

	tailq->head = qnode;
	qnode->prev = &tailq->head;

}

static inline void 
tailq_add(struct tailq *tailq, struct qnode *qnode)
{
	tailq_add_head(tailq, qnode);
}

static inline int
tailq_empty(const struct tailq *tailq)
{
	return !tailq->head;
}

static inline void
tailq_del(struct qnode *qnode)
{
	struct qnode *next = qnode->next;
	struct qnode **prev = qnode->prev;
	*prev = next;
	if (next)
		next->prev = prev;
}

/**
 * tailq_walk - iterate over tailq
 *
 * @self:       container
 * @it:         iterator
 */

#define tailq_walk(self, it) \
	for ((it) = (self)->head; (it) ; (it) = (it)->next)

/**
 * tailq_walk_delsafe - iterate over tailq with safety against removal
 *
 * @tailq:       the your tailq.
 * @it:         iterator
 * @tmp:        tmp iterator
 */

#define tailq_walk_delsafe(self, it, tmp) \
	for ((it) = (self)->head; (it) && ({tmp = (it)->next;1;}); (it) = tmp)

/**
 * tailq_for_each - iterate over tailq
 *
 * @tailq:       the your tailq.
 * @it:         iterator
 * @type:       the structure type
 * @member:     the name of the node within the struct.
 */

#define tailq_for_each(self, it, type, member) \
	for (type *(it) = container_of_safe((self).head, type, member); (it); \
	     (it) = container_of_safe(it->member.next, type, member))

/**
 * tailq_for_each_delsafe - iterate over tailq with safety against removal
 *
 * @tailq:       the your tailq
 * @it:         iterator
 * @type:       the optional structure type
 * @member:     the optional name of the node within the struct.
 */

#define tailq_for_each_delsafe(self, it, type, member) \
	for (type *it = container_of_safe(self.head, type, member), *__it;it &&\
	     ({__it = container_of_safe(it->member.next, type, member);1;}); \
	     it = __it)

__END_DECLS

#endif/*__GENERIC_tailq_H__*/
