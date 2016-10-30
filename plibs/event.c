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
        prv_item_initialize( &events[i].eventItem);

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
    if( xEventGlobalList.earliest_time >  pevent->tag.timestamp){
        xEventGlobalList.earliest_time = pevent->tag.timestamp;
    }
}

void prv_event_delete(ps_event_t * pevent)
{
    prv_list_insert(&pevent->eventItem, &xEventIdleList);
}

void prv_event_tag_set(ps_event_t * pevent, int microstep)
{
	// update the timestamp and init the microstep of event
    ps_servant_t * pservant_src = pevent->pservant_src;
	ps_servant_t * pservant_dest = pevent->pservant_dest;

	//if(pservant_dest->servant_type == 0){
	//	pevent->tag.timestamp = prv_model_time_output_end();
	//	pevent->tag.microstep = 0;
	//}
	if (pservant_dest->servant_type == 1){
		if( pservant_src->servant_type == 0){  // sensor communicate with controller
			pevent->tag.timestamp = prv_model_time_input_end();
		}else{ // controller communicate with controller
			pevent->tag.timestamp = prv_servant_get_start_time(pservant_src) + prv_servant_get_LED(pservant_src);
		}
	}
	else if( pservant_dest->servant_type == 2){ // actuator
		pevent->tag.timestamp = prv_model_time_output_start();

	}else{
		// error happened !!
	}
    pevent->tag.microstep = microstep;
    pevent->tag.level = prv_servant_get_id(pservant_dest);
}


// invoked in prv_event_can_process()
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
	if( pevent->tag.timestamp + led > prv_model_time_output_start()){
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
    // servant_type can't be sensor which only triggered by periodic timing events
	if( servant_type == 2){
		return 2;
	}
    if (servant_type == 1){
		if(prv_event_tag_update(pevent)){
			return 1;
		}else{
			return 0;
		}
	}
}

// wait for specific signal
void ps_event_wait( void * para )
{
    id_t servant_id = * (id_t *) para;
    port_wait(sem[servant_id]);
}


ps_data_t * ps_event_receive()
{
    int len, i;
    ps_event_t * pevent[NUMOFINS];
    item_t * item;
    if( 0 != (len = prv_list_get_length( &xEventReadyList ) ) )
    {
        item                = prv_list_receive( &xEventReadyList );
        pevent[0]           = (ps_event_t *)item->item;
        pevent[0]->data.num = len;
        // set the start time of servant, important !!!!
        prv_servant_set_start_time( prv_ef_get_current_servant(), pevent[0]->tag.timestamp);

        for(i = 1; i < len; ++i){
            item                    = prv_list_receive( &xEventReadyList );
            pevent[i]               = (ps_event_t *)item->item;
            pevent[0]->data.data[i] = pevent[i]->data.data[0];  // integrate events' data

            prv_event_delete(pevent[i]);   // delete left events
        }
        prv_event_delete(pevent[0]); // just change the owner of event, but won't delete the memory of event
        return &pevent[0]->data;
    }
    return NULL;
}


void ps_event_create( ps_data_t * new_data)
{
    int i, num;
    item_t * pitem;
	ps_event_t * pevent_temp;

    ps_servant_t * pservant = prv_ef_get_current_servant();
    num = prv_ef_get_dest_num(pservant);

    port_print("i'm in ps_event_create()\n\r");
    for( i = 0; i < num; ++i ){
        pitem = prv_list_receive(&xEventIdleList);
        pevent_temp = (ps_event_t *)pitem->item;

        pevent_temp->pservant_src  = pservant;
        pevent_temp->pservant_dest = prv_ef_get_ith_dest(pservant,i);

        // update the tag of event according to the timing semantics of PSEFM
        prv_event_tag_set(pevent_temp, i);
        pevent_temp->data.data[0]  = new_data->data[0];
        pevent_temp->data.num = 0;

        prv_event_send(pevent_temp);
    }
}


