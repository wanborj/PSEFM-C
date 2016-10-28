#ifndef __EVENT_H
#define __EVENT_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"

#define prv_event_get_timestamp(pevent) (((ps_event_t *)pevent)->tag.timestamp)
#define prv_event_get_microstep(pevent) (((ps_event_t *)pevent)->tag.microstep)
#define prv_event_get_level(pevent) (((ps_event_t *)pevent)->tag.level)
#define prv_event_get_deadline(pevent) (((ps_event_t *)pevent)->tag.deadline)
#define prv_event_get_data(pevent) (((ps_event_t *)pevent)->data.data)
#define prv_event_get_data_num(pevent) (((ps_event_t *)pevent)->data.num)
#define prv_event_get_src(pevent) (((ps_event_t *)pevent)->pservant_src)
#define prv_event_get_dest(pevent) (((ps_event_t *)pevent)->pservant_dest)
#define prv_event_get_flag(pevent) (((ps_event_t *)pevent)->flag)

//  send event to global event list;
//  invoked in ps_event_create()
void prv_event_send(ps_event_t * pevent);

void prv_event_delete(ps_event_t * pevent);

// event list initialize;
// including blank event lis, global event list,
// local event list and ready event list
void prv_event_list_initialize();

// invoked in prv_event_list_initialize()
void prv_event_initialize();

// invoked in ps_event_create()
// set the tag of events sent by the current servant
void prv_event_tag_set(ps_event_t * pevent);


// judge wether the timestamp of event is legal
int  prv_event_is_overlap(ps_event_t * pevent);


// update the timestamp of event
int prv_event_tag_update(ps_event_t * pevent);


// update xFutureModelTime
void prv_event_future_model_time_reset();

//  judge wether event can be processed
int prv_event_can_process(ps_event_t * pevent);



/* API */
// waiting for periodic timing event or message-arrival event
void ps_event_wait();

// receive events that contain data;
// invoked in servant runnable function
// update servant start_time
ps_event_t * ps_event_receive();



// event create for several destinate servants
// initialize the flag in event
void ps_event_create(ps_event_t * pevent, ps_data_t *data);



#endif
