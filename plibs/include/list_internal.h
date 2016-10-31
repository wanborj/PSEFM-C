#ifndef __LIST_INTERNAL_H
#define __LIST_INTERNAL_H

#include "PSEFMport.h"
#include "PSEFMconfigure.h"



//list operation
#define prv_list_get_length(pevent_list)         (((list_t *)pevent_list)->length)
#define prv_list_get_earliest_time(pevent_list)  (((list_t *)pevent_list)->earliest_time)
#define prv_list_get_first_item(pevent_list)     (((list_t *)pevent_list)->first)
#define prv_list_get_last_item(pevent_list)      (((list_t *)pevent_list)->last)

//item operation
#define prv_item_get_entity(pevent_item)   (((item_t *)pevent_item)->item) // objects like events
#define prv_item_get_owner(pevent_item)    (((item_t *)pevent_item)->owner) // list it belongs to
#define prv_item_get_next(pevent_item)     (((item_t *)pevent_item)->next)
#define prv_item_get_prev(pevent_item)     (((item_t *)pevent_item)->prev)

void prv_list_initialize(list_t * pEventList);

void prv_item_initialize(void * item, item_t * pEventItem);

void prv_list_insert( item_t * pEventItem, list_t * pEventList);

void prv_list_insert_sorted(item_t * pEventItem, list_t * pEventList);

void prv_list_remove(item_t * pEventItem);

void prv_list_earlist_time_update(list_t * pEventList);

// get the first event of the event list
item_t * prv_list_receive(list_t * pEventList);

#endif
