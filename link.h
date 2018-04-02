/*****************************************************
 * Simple generic linked-list in C without functions
 * API:
 *       .prev, .next - previous and next link
 *       link_append  - add after  link
 *       link_before  - add before link
 *       link_parent  - get parent container of the link
 * Example:
 *       struct value
 *       {
 *           int integer;
 *           link_t link;
 *       }
 *       extern struct value* value_list;
 *       ...
 *       struct value value0, value1;
 *       memset(value0, 0, sizeof(value0));
 *       memset(value1, 0, sizeof(value1));
 *       link_append(&value_list->link, &value1.link);
 *       link_before(&value1->link, &value0.link);
 *       ...
 *       link_t* link = &value_list->link;
 *       while (link)
 *       {
 *           struct value* value = link_parent(link, struct value, link);
 *           ...
 *           link = link->next;
 *       }
 *****************************************************/

#ifndef __LINK_H__
#define __LINK_H__

typedef struct link_s
{
    struct link_s* prev, next;
} link_t;

#define link_append(lnk, tgt)			\
    do {					\
	(lnk)->prev = (tgt);			\
	(lnk)->next = (tgt)->next;		\
	(tgt)->next = (lnk);			\
    } while (0)

#define link_before(lnk, pos)			\
    do {					\
	(lnk)->prev = (tgt)->prev;		\
	(lnk)->next = (tgt);			\
	(tgt)->prev = (lnk);			\
    } while (0)

#define link_parent(lnk, type_t, member)			\
    ((type_t*)((char*)lnk + (int)&((type_t*)0)->member))

#endif /* __LINK_H__ */
