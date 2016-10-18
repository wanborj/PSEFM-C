#include "list_internal.h"

void prv_event_list_initialize(struct event_list_t * pEventList)
{
    pEventList->length = 0;
    pEventList->earliest_time = 0;
    pEventList->first = pEventList->last = NULL;
}

void prv_event_item_initialize(struct event_item_t * pEventItem)
{
    pEventItem->owner = NULL;
    pEventItem->next = NULL;
    peventItem->prev = NULL;
}

void prv_event_list_insert_end( struct event_item_t * pEventItem,
                              struct event_list_t * pEventList)
{
    ps_event_t * pevent = prv_item_get_event_entity(pEventItem);

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

    if(pEventList->earliest_time > pevent->tag.xTimestamp){
        pEventList->earliest_time = pevent->tag.xTimestamp;
    }
    pEventList->length ++;
    pEventItem->owner = (void *)pEventList;
}

static int tag_compare(ps_event_t * pe1, ps_event_t *pe2)
{
    if( pe1->tag.xTimestamp < pe2->tag.xTimestamp ){
        return 1;
    }else if( pe1->tag.xTimestamp == pe2->tag.xTimestamp ){
        if( pe1->tag.xMicroStep < pe2->tag.xMicroStep ){
            return 1;
        }else if( pe1->tag.xMicroStep == pe2->tag.xMicroStep ){
            if( pe1->tag.xLevel < pe2->tag.xLevel ){
                return 1;
            }
        }
    }

    return 0;
}

void prv_event_list_insert_sorted(struct event_item_t * pEventItem,
                            struct event_list_t * pEventList)
{
    ps_event_t * pevent = prv_item_get_event_entity(pEventItem);
    volatile  struct event_item_t * pIndex;

    if(pEventList->length == 0){
        pEventList->first = pEventList->last = pEventItem;
        pEventItem->prev = pEventItem;
        pEventItem->next = pEventItem;
    }else{
        pIndex = prv_list_get_first_item(pEventList);
        for(; pIndex != prv_list_get_last_item(pEventList); pIndex = prv_item_get_event_next(pIndex)){
            if(1 == tag_compare(pevent, prv_item_get_event_entity(pIndex))){
                pIndex->prev->next = pEventItem;
                pEventItem->prev = pIndex->prev;

                pEventItem->next = pIndex;
                pIndex->prev = pEventItem;

                // if pEventItem is has smallest model time
                if(pIndex == prv_list_get_first_item(pEventList)){
                    pEventList->first = pEventItem;
                }
                break;
            }
        }
        if( pIndex == prv_list_get_last_item(pEventList) ){
            if(1 == tag_compare(pevent, prv_item_get_event_entity(pIndex))){
                pIndex->prev->next = pEventItem;
                pEventItem->prev = pIndex->prev;

                pEventItem->next = (struct event_item_t *)pIndex;
                pIndex->prev = pEventItem;

            }else{ // insert the pEventItem into the end of pEventList
                pEventList->last->next = pEventItem;
                pEventItem->prev = pEventList->last;
                pEventItem->next = pEventList->first;
                pEventList->last = pEventItem;
            }

        }
    }

    if(pEventList->earliest_time > pevent->tag.xTimestamp){
        pEventList->earliest_time = pevent->tag.xTimestamp;
    }
    pEventList->length ++;
    pEventItem->owner = (void *)pEventList;
}

void prv_event_list_remove(struct event_item_t * pEventItem)
{
    struct event_list_t * pList = (struct event_list_t *)pEventItem->owner;

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

void prv_event_list_earlist_time_update(struct event_list_t * pEventList)
{
    volatile  struct event_item_t * pIndex = prv_list_get_first_item(pEventList);
    pEventList->earliest_time = pIndex->item->tag.xTimestamp;

    for(; pIndex != prv_list_get_first_item(pEventList); pIndex = prv_item_get_event_next(pEventList)){
        if(pEventList->earliest_time > pIndex->item->tag.xTimestamp)
        {
            pEventList->earliest_time = pIndex->item->tag.xTimestamp;
        }
    }
}
