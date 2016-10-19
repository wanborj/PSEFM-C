#ifndef __LIST_INTERNAL_H
#define __LIST_INTERNAL_H

#include "PSEFMport.h"
#include "PSEFMconfigure.h"



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

void prv_list_initialize(list_t * pEventList);

void prv_item_initialize(item_t * pEventItem);

void prv_list_insert( item_t * pEventItem, list_t * pEventList);

void prv_list_insert_sorted(item_t * pEventItem, list_t * pEventList);

void prv_list_remove(item_t * pEventItem);

void prv_list_earlist_time_update(list_t * pEventList);

// get the first event of the event list
item_t * prv_list_receive(list_t * pEventList);

#endif
