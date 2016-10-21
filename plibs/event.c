#include "event.h"
#include "exec_flow.h"
#include "list_internal.h"


extern ps_servant_t * pcurrent_servant;

list_t xEventIdleList;
list_t xEventGlobalList;
list_t xEventLocalList;
list_t xEventReadyList;
ps_event_t events[NUMOFRELATIONS];
ps_event_sem_t sem[NUMOFSERVANTS];

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

// know the dest servant of pevent, and send the event to each of the dest servants
void prv_event_send(ps_event_t *pevent)
{
    prv_list_insert(&pevent->eventItem, &xEventGlobalList);
}

void prv_event_delete(ps_event_t * pevent)
{
    prv_list_insert(&pevent->eventItem, &xEventIdleList);
}

// wait for specific signal
void ps_event_wait()
{
    id_t current_servant_id = pcurrent_servant->servant_id;
    port_wait_event(sem[servant_id]);
}


ps_event_t * ps_event_receive()
{
    int len, i;
    ps_event_t * pevent[NUMOFINS];
    item_t * item;
    if( 0 != (len = prv_list_get_length(&xEventReadyList) ) )
    {
        item = prv_list_receive( &xEventReadyList );
        pevent[0] = (ps_event_t *)item->item;
        pevent[0]->data.num = len;
        for(i = 1; i < len; ++i){
            item = prv_list_receive( &xEventReadyList );
            pevent[i] = (ps_event_t *)item->item;
            pevent[0]->data.data[i] = pevent[i]->data.data[0];  // integrate events' data
            prv_event_delete(pevent[i]);   // delete left events
        }
        return pevent[0];
    }
    return NULL;
}

void ps_event_create(ps_tag_t *tag, ps_data_t *data)
{
    int i, num;
    item_t * pitem;
    ps_event_t * pevent;
    ps_servant_t * pcurrent_servant = prv_ef_get_currente_servant();
    num = prv_ef_get_dest_num(pcurrent_servant);

    for( i = 0; i < num; ++i ){
        pitem = prv_list_receive(&xEventIdleList);
        pevent = (ps_event_t *)pitem->item;

        pevent->tag.timestamp = tag->timestamp;
        pevent->tag.microstep = tag->microstep;
        pevent->tag.level     = tag->level;
        pevent->tag.deadline  = tag->deadline;
        pevent->data.data[0] = data->data[0];

        pevent->pservant_src = pcurrent_servant;
        pevent->pservant_dest = prv_ef_get_ith_dest(i);

        prv_event_send(pevent);
    }
}


