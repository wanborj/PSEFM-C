#include "list_internal.h"
#include "event.h"

void prv_list_initialize(list_t * pEventList)
{
    pEventList->length = 0;
    pEventList->earliest_time = 0;
    pEventList->first = pEventList->last = NULL;
}

void prv_item_initialize(item_t * pEventItem)
{
    pEventItem->owner = NULL;
    pEventItem->next = NULL;
    pEventItem->prev = NULL;
}

void prv_list_insert( item_t * pEventItem,
                              list_t * pEventList)
{
    ps_event_t * pevent = (ps_event_t *) prv_item_get_entity(pEventItem);

    if(pEventList->length == 0){
        pEventList->first = pEventList->last = pEventItem;
        pEventItem->prev = pEventItem;
        pEventItem->next = pEventItem;
    }else{
        pEventList->last->next = pEventItem;
        pEventItem->prev = pEventList->last;
        pEventItem->next = pEventList->first;
        pEventList->last = pEventItem;
    }

    if(pEventList->earliest_time > pevent->tag.timestamp){
        pEventList->earliest_time = pevent->tag.timestamp;
    }
    pEventList->length ++;
    pEventItem->owner = (void *)pEventList;
}

static int tag_compare(ps_event_t * pe1, ps_event_t *pe2)
{
    if( pe1->tag.timestamp < pe2->tag.timestamp ){
        return 1;
    }else if( pe1->tag.timestamp == pe2->tag.timestamp ){
        if( pe1->tag.microstep < pe2->tag.microstep ){
            return 1;
        }else if( pe1->tag.microstep == pe2->tag.microstep ){
            if( pe1->tag.level < pe2->tag.level ){
                return 1;
            }
        }
    }

    return 0;
}

void prv_list_insert_sorted(item_t * pEventItem, list_t * pEventList)
{
    ps_event_t * pevent = (ps_event_t *)prv_item_get_entity(pEventItem);
    volatile  item_t * pIndex;

    if(pEventList->length == 0){
        pEventList->first = pEventList->last = pEventItem;
        pEventItem->prev = pEventItem;
        pEventItem->next = pEventItem;
    }else{
        pIndex = prv_list_get_first_item(pEventList);
        for(; pIndex != prv_list_get_last_item(pEventList); pIndex = prv_item_get_next(pIndex)){
            if(1 == tag_compare(pevent, prv_item_get_entity(pIndex))){
                pIndex->prev->next = pEventItem;
                pEventItem->prev = pIndex->prev;

                pEventItem->next = (item_t *)pIndex;
                pIndex->prev = pEventItem;

                // if pEventItem is has smallest model time
                if(pIndex == prv_list_get_first_item(pEventList)){
                    pEventList->first = pEventItem;
                }
                break;
            }
        }
        if( pIndex == prv_list_get_last_item(pEventList) ){
            if(1 == tag_compare(pevent, prv_item_get_entity(pIndex))){
                pIndex->prev->next = pEventItem;
                pEventItem->prev = pIndex->prev;

                pEventItem->next = (item_t *)pIndex;
                pIndex->prev = pEventItem;

            }else{ // insert the pEventItem into the end of pEventList
                pEventList->last->next = pEventItem;
                pEventItem->prev = pEventList->last;
                pEventItem->next = pEventList->first;
                pEventList->last = pEventItem;
            }
        }
    }

    if(pEventList->earliest_time > pevent->tag.timestamp){
        pEventList->earliest_time = pevent->tag.timestamp;
    }
    pEventList->length ++;
    pEventItem->owner = (void *)pEventList;
}

void prv_list_remove(item_t * pEventItem)
{
    list_t * pList = (list_t *)pEventItem->owner;

    if(pList->length == 1){
        pList->first = NULL;
        pList->last = NULL;
    }else{
        if(pEventItem == prv_list_get_first_item(pList)){
            pList->first = pEventItem->next;
        }
        if(pEventItem == prv_list_get_last_item(pList)){
            pList->last = pEventItem->prev;
        }
    }

    pEventItem->prev->next = pEventItem->next;
    pEventItem->next->prev = pEventItem->prev;
    pEventItem->owner = NULL;
    pList->length --;
}


void prv_list_earlist_time_update(list_t * pEventList)
{
    volatile  item_t * pIndex = prv_list_get_first_item(pEventList);
    pEventList->earliest_time = ((ps_event_t *)pIndex->item)->tag.timestamp;

    for(; pIndex != prv_list_get_first_item(pEventList); pIndex = prv_item_get_next(pIndex)){
        if(pEventList->earliest_time > ((ps_event_t *)pIndex->item)->tag.timestamp)
        {
            pEventList->earliest_time = ((ps_event_t *)pIndex->item)->tag.timestamp;
        }
    }
}


item_t * prv_list_receive(list_t * pEventList)
{
    item_t * pitem = pEventList->first;
    prv_list_remove(pitem);

    return pitem;
}


