#include "event.h"
#include "exec_flow.h"
#include "list_internal.h"

static tick_t xFutureModelTime;

list_t xEventIdleList;
list_t xEventGlobalList;
list_t xEventLocalList;
list_t xEventReadyList;
ps_event_t events[NUMOFRELATIONS];
ps_event_sem_t sem[NUMOFSERVANTS];

static void prv_event_sem_initialize()
{
	int i;
	for( i = 0; i < NUMOFSERVANTS; ++ i){
        port_pure_event_create( sem[i] );
        port_wait( sem[i] );
	}
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

	prv_event_sem_initialize();  // init the semaphores
}

void prv_event_list_initialize()
{
    prv_list_initialize(&xEventIdleList);
    prv_list_initialize(&xEventGlobalList);
    prv_list_initialize(&xEventLocalList);
    prv_list_initialize(&xEventReadyList);

    prv_event_initialize();
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

void prv_event_tag_set(ps_event_t * pevent)
{
	// update the timestamp and init the microstep of event
	ps_servant_t * pservant = prv_ef_get_current_servant();
	ps_servant_t * pservant_dest = prv_ef_get_ith_dest(pservant,0);

	if(pservant_dest->servant_type == 0){
		pevent->tag.timestamp = prv_model_time_output_end();
		pevent->tag.microstep = 0;
	}
	else if (pservant_dest->servant_type == 1){
		if( pservant->servant_type == 0){  // sensor communicate with controller
			pevent->tag.timestamp = prv_model_time_input_end();
			pevent->tag.microstep = 0;
		}else{ // controller communicate with controller

			pevent->tag.timestamp = prv_servant_get_start_time(pservant) + prv_servant_get_LED(pservant);
			pevent->tag.microstep = 0;
		}
	}
	else if( pservant_dest->servant_type == 2){ // actuator
		pevent->tag.timestamp = prv_model_time_output_start();
		pevent->tag.microstep = 0;
	}else{
		// error happened !!
	}
}


// invoked in prv_ef_trigger
int prv_event_tag_update(ps_event_t * pevent)
{
	pevent->tag.timestamp = xFutureModelTime;
	tick_t led = pevent->pservant_dest->LED;
	if( !prv_event_is_overlap(pevent) && xFutureModelTime < prv_model_time_output_start()){
		// process event pevent at the xFuturemodelTime
		xFutureModelTime += led;
		return 1; // update success
	}else if (prv_event_is_overlap(pevent) && xFutureModelTime < prv_model_time_output_start()){
		xFutureModelTime = prv_model_time_output_start()+INPUT+OUTPUT;
		pevent->tag.timestamp = xFutureModelTime;
		xFutureModelTime += led;
		return 0;
	}else{
		xFutureModelTime += led;
		return 0;
	}
}

void prv_event_future_model_time_reset()
{
	xFutureModelTime = port_get_current_time() + INPUT;
}


// 1 for overlap ; 0 for not overlap
int  prv_event_is_overlap(ps_event_t * pevent)
{
	tick_t led = pevent->pservant_dest->LED;
	if( pevent->tag.timestamp+ led > prv_model_time_output_start()){
		return 1;
	}else{
		return 0;
	}
}

// 2 for I/O
// 1 for controller
// 0 for no valid
int prv_event_can_process(ps_event_t * pevent)
{
	int servant_type = pevent->pservant_dest->servant_type;
	if(servant_type == 0 || servant_type == 2){
		return 2;
	}else{
		if(prv_event_tag_update(pevent)){
			return 1;
		}else{
			return 0;
		}
	}
}

// wait for specific signal
void ps_event_wait()
{
    ps_servant_t * pservant = prv_ef_get_current_servant();
    id_t current_servant_id = pservant->servant_id;
    port_wait(sem[current_servant_id]);
}


ps_event_t * ps_event_receive()
{
    int len, i;
    ps_event_t * pevent[NUMOFINS];
    item_t * item;
    if( 0 != (len = prv_list_get_length( &xEventReadyList ) ) )
    {
        item                = prv_list_receive( &xEventReadyList );
        pevent[0]           = (ps_event_t *)item->item;
        pevent[0]->data.num = len;

        for(i = 1; i < len; ++i){
            item                    = prv_list_receive( &xEventReadyList );
            pevent[i]               = (ps_event_t *)item->item;
            pevent[0]->data.data[i] = pevent[i]->data.data[0];  // integrate events' data

            prv_event_delete(pevent[i]);   // delete left events
        }
        return pevent[0];
    }
    return NULL;
}


void ps_event_create(ps_event_t * pevent, ps_data_t * new_data)
{
    int i, num;
    item_t * pitem;
    ps_tag_t * tag;
	ps_event_t * pevent_temp;
	prv_event_tag_set(pevent);  // update the tag of event

    ps_servant_t * pservant = prv_ef_get_current_servant();
    num = prv_ef_get_dest_num(pservant);

    for( i = 0; i < num; ++i ){
        pitem = prv_list_receive(&xEventIdleList);
        pevent_temp = (ps_event_t *)pitem->item;

        pevent_temp->pservant_src = pservant;
        pevent_temp->pservant_dest = prv_ef_get_ith_dest(pservant,i);

        pevent_temp->tag.timestamp = pevent->tag.timestamp;
        pevent_temp->tag.microstep = pevent->tag.microstep+i;
        pevent_temp->tag.level     = prv_servant_get_id(pevent_temp->pservant_dest);  // set the level of event as the id of dest servant
        pevent_temp->tag.deadline  = pevent->tag.deadline;  // nothing here yet
        pevent_temp->data.data[0]  = new_data->data[0];

        prv_event_send(pevent_temp);
    }
	prv_event_delete(pevent);
}


