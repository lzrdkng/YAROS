/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file dlist.h
 *
 * @brief Copycat of the Linux Kernel double linked list
 * implementation. Cuz it get the job done.
 *
 * @sa "linux/include/linux/list.h"
 */
#ifndef _URBAN_DLIST_H
#define _URBAN_DLIST_H


#define offsetof(st, m) \
     ((size_t) ( (char *)&((st *)(0))->m - (char *)0 ))



/*============================================================================+
 |                                  Structure                                 |
 +============================================================================*/
struct dlist {
	struct dlist *prev, *next;
};



/*============================================================================+
 |                     Double Linked lists Manipulations                      |
 +============================================================================*/

/*----------------------------dlist initialization----------------------------*/
#define DLIST_INIT(name)						\
	{&(name), &(name)}


#define DLIST(name)								\
	struct dlist name = DLIST_INIT(name)


static inline void __attribute__((always_inline))
INIT_DLIST(struct dlist *dlist)
{
	dlist->prev = dlist;
	dlist->next = dlist;
}





/*-----------------------------Internal Use Only-----------------------------*/
static inline void __attribute__((always_inline))
__dlist_add(struct dlist *new,
            struct dlist *prev,
            struct dlist *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}


static inline void __attribute__((always_inline))
__dlist_del(struct dlist *prev,
            struct dlist *next)
{
	next->prev = prev;
	prev->next = next;
}


static inline void __attribute__((always_inline))
__dlist_splice(struct dlist *dlist,
               struct dlist *prev,
               struct dlist *next)
{
	struct dlist *first = dlist->next;
	struct dlist *last  = dlist->prev;

	prev->next  = first;
	first->prev = prev;

	next->prev = last;
	last->next = next;
}




/*  ------------------------- dlist manipulations --------------------------  */
static inline void __attribute__((always_inline))
dlist_add(struct dlist *new,
		  struct dlist *head)
{
	__dlist_add(new, head, head->next);
}


static inline void __attribute__((always_inline))
dlist_add_tail(struct dlist *new,
			   struct dlist *head)
{
	__dlist_add(new, head->prev, head);
}


static inline void __attribute__((always_inline))
dlist_del(struct dlist *entry)
{
	__dlist_del(entry->prev, entry->next);

	entry->prev = NULL;
	entry->next = NULL;
}


static inline void __attribute__((always_inline))
dlist_move(struct dlist *dlist,
		   struct dlist *head)
{
	__dlist_del(dlist->prev, dlist->next);
	dlist_add(dlist, head);
}


static inline void __attribute__((always_inline))
dlist_move_tail(struct dlist *dlist,
				struct dlist *head)
{
	dlist_del(dlist);
	dlist_add_tail(dlist, head);
}


static inline void __attribute__((always_inline))
dlist_replace(struct dlist *old,
			  struct dlist *new)
{
	new->next = old->next;
	new->prev = old->prev;

	new->next->prev = new;
	new->prev->next = new;

	/*
	 * Make sure that the old entry doesn't point back to the list.
	 */
	old->prev = NULL;
	old->next = NULL;
}


static inline char __attribute__((always_inline))
dlist_is_last(const struct dlist *dlist,
              const struct dlist *head)
{
	return dlist->next == head;
}


static inline char __attribute__((always_inline))
dlist_is_empty(const struct dlist *head)
{
	return head == head->next;
}


static inline char __attribute__((always_inline))
dlist_is_singular(const struct dlist *head)
{
	return !dlist_is_empty(head) && (head->next == head->prev);
}


static inline void __attribute__((always_inline))
dlist_rotate_left(struct dlist *head)
{
	if (!dlist_is_empty(head))
		dlist_move_tail(head->next, head);
}



static inline void __attribute__((always_inline))
dlist_splice(struct dlist *dlist,
			 struct dlist *head)
{
	if (!dlist_is_empty(dlist))
		__dlist_splice(dlist, head, head->next);
}


static inline void __attribute__((always_inline))
dlist_splice_tail(struct dlist *dlist,
				  struct dlist *head)
{
	if (!dlist_is_empty(dlist))
		__dlist_splice(dlist, head, head->prev);
}




/*  ----------------------------- Entry Access -----------------------------  */

/*
 * From drivers/gpu/drm/nouveau/include/nvif/dlist.h
 */
#define dlist_entry(ptr, type, member)					\
	((type *) ((char *)(ptr) - offsetof(type, member)))


#define dlist_first_entry(ptr, type, member)	\
	dlist_entry((ptr)->next, type, member)


#define dlist_last_entry(ptr, type, member)		\
	dlist_entry((ptr)->prev, type, member)


#define dlist_next_entry(pos, member)						\
	dlist_entry((pos)->member.next, typeof(*(pos)), member)


#define dlist_prev_entry(pos, member)						\
	dlist_entry((pos)->member.prev, typeof(*(pos)), member)


#define dlist_for_each(pos, head)							\
	for (pos = head->next; pos != head; pos = pos->next)


#define dlist_for_each_prev(pos, head)						\
	for (pos = head->prev; pos != head; pos = pos->prev)


#define dlist_for_each_entry(pos, head, member)	\
	for (pos = dlist_first_entry(head, typeof(*pos), member);\
			 &(pos->member) != head; \
		 pos = dlist_next_entry(pos, member))

#define dlist_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define dlist_for_each_entry_prev(pos, head, member)			\
	for (pos = dlist_last_entry(head, typeof(*pos), member);	\
		 &(pos->member) != head;								\
		 pos = dlist_prev_entry(pos, member))

#define dlist_for_each_entry_safe(pos, n, head, member)			\
	for (pos = dlist_first_entry(head, typeof(*pos), member),	\
		n = dlist_next_entry(pos, member);			\
	     &pos->member != (head); 					\
	     pos = n, n = dlist_next_entry(n, member))

#define dlist_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = dlist_last_entry(head, typeof(*pos), member),		\
		n = dlist_prev_entry(pos, member);			\
	     &pos->member != (head); 					\
	     pos = n, n = dlist_prev_entry(n, member))

#endif // _URBAN_DLIST_H
