#ifndef __LIST_INTERNAL_H
#define __LIST_INTERNAL_H

#include "PSEFMport.h"
#include "event.h"

struct event_list_t{
    int                 length;
    tick_t              earliest_time; // the earliest time-stamp of event in a event list
    struct event_item_t *first;
    struct event_item_t *last;
};

// event list struct
struct event_item_t{
    ps_event_t * item;   // the object item, event or servant
    void * owner;   // the list it belongs to
    struct event_item_t *next;
    struct event_item_t *prev;
};


//list operation
#define prv_list_get_length(el)         (el->length)
#define prv_list_get_earliest_time(el)  (el->earliest_time)
#define prv_list_get_first_item(el)     (el->first)
#define prv_list_get_last_item(el)      (el->last)

//item operation
#define prv_item_get_event_entity(ei)   (ei->item)
#define prv_item_get_event_owner(ei)    (ei->owner)
#define prv_item_get_event_next(ei)     (ei->next)
#define prv_item_get_event_prev(ei)     (ei->prev)

void prv_event_list_initialize(struct event_list_t * pEventList);

void prv_event_item_initialize(struct event_item_t * pEventItem);

void prv_event_list_insert( struct event_item_t * pEventItem,
                              struct event_list_t * pEventList);

void prv_event_list_insert_sorted(struct event_item_t * pEventItem,
                            struct event_list_t * pEventList);

void prv_event_list_remove(struct event_item_t * pEventItem);

void prv_event_list_earlist_time_update(struct event_list_t * pEventList);

#endif
