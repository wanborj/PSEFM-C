#ifndef __EVENT_H
#define __EVENT_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"

typedef struct {
    tick_t xDeadline;     /*< RM. The smaller the period the task is , the higher priority the event is. >*/
    tick_t xTimestamp;             /*< the time to be proecessed >*/
    int xMicroStep;            /*< the topology order >*/
    int xLevel;                /*< the depth of current servant in a task >*/
}ps_tag_t;

typedef struct {
    /* data type can be changed here. Data type including portCHAR, portFLOAT, portLONG, portSHORT, portBASE_TYPE*/
    double data;
}ps_data_t;

typedef struct {
    ps_task_t * pservant_src;
    ps_task_t * pservant_dest;
    ps_tag_t  tag;
    ps_data_t data;
    struct event_item_t eventItem;
    int flag; // for counting the "arrive" number in servant;
              // if counted, then flag ==1 ; else flag == 0
}ps_event_t;


// event list operation


#endif
