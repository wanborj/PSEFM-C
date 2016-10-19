#ifndef __EVENT_H
#define __EVENT_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"
#include "list_internal.h"



// event operation

// waiting for periodic timing event or message-arrival event
void ps_event_wait();

// receive events that contain data;
// invoked in servant runnable function
// update servant start_time
ps_event_t * ps_event_receive();

// event create for several destinate servants
// initialize the flag in event
void ps_event_create(ps_tag_t tag, ps_data_t data);

//  send event to global event list;
//  invoked in ps_event_create()
void prv_event_send(ps_event_t * pevent);

void prv_event_delete(ps_event_t * pevent);

// event list initialize;
// including blank event lis, global event list,
// local event list and ready event list
void prv_event_list_initialize();

void prv_event_initialize();

#endif
