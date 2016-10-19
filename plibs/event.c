#include "event.h"


extern ps_servant_t * pcurrent_servant;

static list_t xEventIdleList;
static list_t xEventGlobalList;
static list_t xEventLocalList;
static list_t xEventReadyList;
ps_event_t events[NUMOFRELATIONS];

void prv_event_list_initialize()
{
    prv_list_initialize(&xEventIdleList);
    prv_list_initialize(&xEventGlobalList);
    prv_list_initialize(&xEventLocalList);
    prv_list_initialize(&xEventReadyList);

    prv_event_initialize();
}

void prv_event_initialize()
{
    int i;
    for( i = 0; i < NUMOFRELATIONS; ++i ){
        events[i].pservant_src = NULL;
        events[i].pservant_dest = NULL;
        events[i].flag = 0;
        prv_item_initialize(& events[i].eventItem);

        prv_list_insert(&events[i].eventItem, &xEventIdleList);
    }
}

void prv_event_send(ps_event_t *pevent)
{
    prv_list_remove(&pevent->eventItem);
    prv_list_insert(&pevent->eventItem, &xEventGlobalList);
}

void prv_event_delete(ps_event_t * pevent)
{
    prv_list_remove(&pevent->eventItem);
    prv_list_insert(&pevent->eventItem, &xEventIdleList);
}

void ps_event_wait()
{

}

ps_event_t * ps_event_receive()
{

}

void ps_event_create(ps_tag_t tag, ps_data_t data)
{
    item_t * pitem = prv_list_receive(&xEventIdleList);
    ps_event_t * pevent = (ps_event_t *)pitem->item;

    pevent->tag = tag;
    pevent->data = data;
    pevent->pservant_src = pcurrent_servant;

    prv_ef_set_dest( pevent );
    prv_event_send(pevent);
}


